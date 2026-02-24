#include "reader.h"

#include "client.h"
#include "input_prompt.h"

#include <iostream>

using namespace boost::asio;
using namespace std::placeholders;

Reader::Reader(const DescriptorType::executor_type &executor, Client &client, InputPrompt &prompt)
    : m_stdin_desc{executor, ::dup(STDIN_FILENO)}
    , m_client{client}
    , m_prompt{prompt}
{
}

void Reader::run()
{
  setup_topic_leaf();
}

void Reader::setup_topic_leaf()
{
  m_topic_leaf.clear();
  m_prompt.set_and_diplay("Enter topic leaf (empty to end client)");
  async_read_until(m_stdin_desc, m_buffer, '\n', std::bind(&Reader::handle_topic_leaf, this, _1, _2));
}

void Reader::setup_content()
{
  m_content.clear();
  m_prompt.set_and_diplay("Enter content");
  async_read_until(m_stdin_desc, m_buffer, '\n', std::bind(&Reader::handle_content, this, _1, _2));
}

void Reader::setup_retain()
{
  m_retain = false;
  m_prompt.set_and_diplay("Retain ? (1=yes, 0=no)");
  async_read_until(m_stdin_desc, m_buffer, '\n', std::bind(&Reader::handle_retain, this, _1, _2));
}

void Reader::setup_qos()
{
  m_qos = 0;
  m_prompt.set_and_diplay("QoS (0..2)");
  async_read_until(m_stdin_desc, m_buffer, '\n', std::bind(&Reader::handle_qos, this, _1, _2));
}

void Reader::handle_topic_leaf(boost::system::error_code err, std::size_t read_size)
{
  if (err) {
    std::cout << "handle topic leaf failed. error=" << err.message() << std::endl;
    setup_topic_leaf();
    return;
  }

  if (read_size == 1) {
    m_client.disconnect();
    return;
  }
  m_topic_leaf = read_buffer_line();

  setup_content();
}

void Reader::handle_content(boost::system::error_code err, std::size_t read_size)
{
  if (err) {
    std::cout << "handle content failed! error=" << err.message() << std::endl;
    setup_content();
    return;
  }
  m_content = read_buffer_line();

  setup_retain();
}

void Reader::handle_retain(boost::system::error_code err, std::size_t read_size)
{
  if (err) {
    std::cout << "handle retain failed! error=" << err.message() << std::endl;
    setup_retain();
    return;
  }

  const auto retain_str = read_buffer_line();
  if (retain_str == "0") {
    m_retain = false;
  } else if (retain_str == "1") {
    m_retain = true;
  } else {
    std::cout << "Wrong input: " << retain_str << std::endl;
    setup_retain();
    return;
  }

  setup_qos();
}

void Reader::handle_qos(boost::system::error_code err, std::size_t read_size)
{
  if (err) {
    std::cout << "handle retain failed! error=" << err.message() << std::endl;
    setup_retain();
    return;
  }

  const auto qos_str = read_buffer_line();
  if (qos_str == "0") {
    m_qos = 0;
  } else if (qos_str == "1") {
    m_qos = 1;
  } else if (qos_str == "2") {
    m_qos = 2;
  } else {
    std::cout << "Wrong input: " << qos_str << std::endl;
    setup_qos();
    return;
  }

  m_prompt.clear();
  m_client.publish_chat(m_topic_leaf, m_content, m_retain, m_qos, std::bind(&Reader::run, this));
}

std::string Reader::read_buffer_line()
{
  std::istream is(&m_buffer);
  std::string line;
  std::getline(is, line);
  return line;
}
