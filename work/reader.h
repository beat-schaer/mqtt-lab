#pragma once

#include <boost/asio.hpp>

class Client;
class InputPrompt;

class Reader
{
public:
  using DescriptorType = boost::asio::posix::stream_descriptor;

  Reader(const DescriptorType::executor_type &executor, Client &client, InputPrompt &prompt);
  void run();

private:
  void setup_topic_leaf();
  void setup_content();
  void setup_retain();
  void setup_qos();
  void handle_topic_leaf(boost::system::error_code err, std::size_t read_size);
  void handle_content(boost::system::error_code err, std::size_t read_size);
  void handle_retain(boost::system::error_code err, std::size_t read_size);
  void handle_qos(boost::system::error_code err, std::size_t read_size);

  std::string read_buffer_line();

  DescriptorType m_stdin_desc;
  Client &m_client;
  InputPrompt &m_prompt;
  boost::asio::streambuf m_buffer;
  std::string m_topic_leaf{};
  std::string m_content{};
  bool m_retain{};
  int m_qos{};
};