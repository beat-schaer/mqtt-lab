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

  template <typename Reader>
  using InputHandleFn = void (Reader::*)(std::string);
  template <typename Reader>
  using SetupFn = void (Reader::*)();

  template <typename Reader>
  void setup(std::string_view prompt, SetupFn<Reader> this_setup, InputHandleFn<Reader> input_handler);

  template <typename Reader>
  void handle(boost::system::error_code err, std::size_t read_size, SetupFn<Reader> this_setup,
              InputHandleFn<Reader> input_handler);

  std::string read_buffer_line();

  template <typename Reader>
  Reader *downcasted_this()
  {
    return static_cast<Reader *>(this);
  }

  DescriptorType m_stdin_desc;
  Client &m_client;
  InputPrompt &m_prompt;
  boost::asio::streambuf m_buffer;
};

template <typename Reader>
void ReaderBase::setup(std::string_view prompt, SetupFn<Reader> this_setup, InputHandleFn<Reader> input_handler)
{
  m_prompt.set_and_diplay(prompt);
  async_read_until(
      m_stdin_desc, m_buffer, '\n',
      std::bind(&ReaderBase::handle<Reader>, this, _1, _2, std::move(this_setup), std::move(input_handler)));
}

template <typename Reader>
void ReaderBase::handle(boost::system::error_code err, std::size_t read_size, SetupFn<Reader> this_setup,
                        InputHandleFn<Reader> input_handler)
{
  if (err) {
    std::cout << "handle failed! error=" << err.message() << std::endl;
    (downcasted_this<Reader>()->*this_setup)();
    return;
  }
  (downcasted_this<Reader>()->*input_handler)(read_buffer_line());
}
