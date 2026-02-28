#pragma once

#include "input_prompt.h"

#include <boost/asio.hpp>
#include <string_view>

class Client;

using namespace std::placeholders;

class ReaderBase
{
public:
  using DescriptorType = boost::asio::posix::stream_descriptor;

protected:
  ReaderBase(const DescriptorType::executor_type &executor, Client &client, InputPrompt &prompt);

  template <typename PublishReader>
  using InputHandleFn = void (PublishReader::*)(std::string);
  template <typename PublishReader>
  using SetupFn = void (PublishReader::*)();

  template <typename PublishReader>
  void setup(std::string_view prompt, SetupFn<PublishReader> this_setup, InputHandleFn<PublishReader> input_handler);

  template <typename PublishReader>
  void handle(boost::system::error_code err, std::size_t read_size, SetupFn<PublishReader> this_setup,
              InputHandleFn<PublishReader> input_handler);

  std::string read_buffer_line();

  template <typename PublishReader>
  PublishReader *downcasted_this()
  {
    return static_cast<PublishReader *>(this);
  }

  DescriptorType m_stdin_desc;
  Client &m_client;
  InputPrompt &m_prompt;
  boost::asio::streambuf m_buffer;
};

template <typename PublishReader>
void ReaderBase::setup(std::string_view prompt, SetupFn<PublishReader> this_setup,
                       InputHandleFn<PublishReader> input_handler)
{
  m_prompt.set_and_diplay(prompt);
  async_read_until(
      m_stdin_desc, m_buffer, '\n',
      std::bind(&ReaderBase::handle<PublishReader>, this, _1, _2, std::move(this_setup), std::move(input_handler)));
}

template <typename PublishReader>
void ReaderBase::handle(boost::system::error_code err, std::size_t read_size, SetupFn<PublishReader> this_setup,
                        InputHandleFn<PublishReader> input_handler)
{
  if (err) {
    std::cout << "handle failed! error=" << err.message() << std::endl;
    (downcasted_this<PublishReader>()->*this_setup)();
    return;
  }
  (downcasted_this<PublishReader>()->*input_handler)(read_buffer_line());
}
