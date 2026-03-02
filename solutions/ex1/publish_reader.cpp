#include "publish_reader.h"

#include "client.h"
#include "input_prompt.h"

#include <iostream>

using namespace boost::mqtt5;

PublishReader::PublishReader(const DescriptorType::executor_type &executor, Client &client, InputPrompt &prompt)
    : ReaderBase{executor, client, prompt}
{
}

void PublishReader::run()
{
  setup_topic_leaf();
}

void PublishReader::setup_topic_leaf()
{
  setup("Enter topic leaf (empty to end client)", &PublishReader::setup_topic_leaf, &PublishReader::handle_topic_leaf);
}

void PublishReader::setup_content()
{
  setup("Enter content", &PublishReader::setup_content, &PublishReader::handle_content);
}

void PublishReader::setup_retain()
{
  setup("Retain ? (1=yes, 0=no)", &PublishReader::setup_retain, &PublishReader::handle_retain);
}

void PublishReader::setup_qos()
{
  setup("QoS (0..2)", &PublishReader::setup_qos, &PublishReader::handle_qos);
}

void PublishReader::handle_topic_leaf(std::string input)
{
  if (input.empty()) {
    m_client.disconnect();
    return;
  }
  m_topic_leaf = std::move(input);

  setup_content();
}

void PublishReader::handle_content(std::string input)
{
  m_content = std::move(input);

  setup_retain();
}

void PublishReader::handle_retain(std::string input)
{
  if (input == "0") {
    m_retain = retain_e::no;
  } else if (input == "1") {
    m_retain = retain_e::yes;
  } else {
    std::cout << "Wrong input: " << input << std::endl;
    setup_retain();
    return;
  }

  setup_qos();
}

void PublishReader::handle_qos(std::string input)
{
  if (input == "0") {
    m_qos = qos_e::at_most_once;
  } else if (input == "1") {
    m_qos = qos_e::at_least_once;
  } else if (input == "2") {
    m_qos = qos_e::exactly_once;
  } else {
    std::cout << "Wrong input: " << input << std::endl;
    setup_qos();
    return;
  }

  m_prompt.clear();
  m_client.publish_chat(m_topic_leaf, m_content, m_retain, m_qos);

  setup_topic_leaf();
}
