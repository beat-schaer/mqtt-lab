#include "subscribe_reader.h"

#include "client.h"

using namespace boost::mqtt5;

SubscribeReader::SubscribeReader(const DescriptorType::executor_type &executor, Client &client, InputPrompt &prompt,
                                 NextRunner next_runner)
    : ReaderBase{executor, client, prompt}
    , m_next_runner{std::move(next_runner)}
{
}

void SubscribeReader::run()
{
  setup_max_qos();
}

void SubscribeReader::setup_max_qos()
{
  setup("Max QoS (0..2)", &SubscribeReader::setup_max_qos, &SubscribeReader::handle_max_qos);
}

void SubscribeReader::setup_no_local()
{
  setup("No local ? (1=yes, 0=no)", &SubscribeReader::setup_no_local, &SubscribeReader::handle_no_local);
}

void SubscribeReader::setup_retain_as_published()
{
  setup("Retain as published ? (1=yes, 0=no)", &SubscribeReader::setup_retain_as_published,
        &SubscribeReader::handle_retain_as_published);
}

void SubscribeReader::handle_max_qos(std::string input)
{
  if (input == "0") {
    m_max_qos = qos_e::at_most_once;
  } else if (input == "1") {
    m_max_qos = qos_e::at_least_once;
  } else if (input == "2") {
    m_max_qos = qos_e::exactly_once;
  } else {
    std::cout << "Wrong input: " << input << std::endl;
    setup_max_qos();
    return;
  }
  setup_no_local();
}

void SubscribeReader::handle_no_local(std::string input)
{
  if (input == "0") {
    m_no_local = no_local_e::no;
  } else if (input == "1") {
    m_no_local = no_local_e::yes;
  } else {
    std::cout << "Wrong input: " << input << std::endl;
    setup_no_local();
    return;
  }

  setup_retain_as_published();
}

void SubscribeReader::handle_retain_as_published(std::string input)
{
  if (input == "0") {
    m_retain_as_published = retain_as_published_e::dont;
  } else if (input == "1") {
    m_retain_as_published = retain_as_published_e::retain;
  } else {
    std::cout << "Wrong input: " << input << std::endl;
    setup_retain_as_published();
    return;
  }

  m_client.subscribe_myself(m_max_qos, m_no_local, m_retain_as_published);
  m_next_runner();
}
