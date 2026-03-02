#pragma once

#include "reader_base.h"

#include <boost/mqtt5.hpp>

class PublishReader : public ReaderBase
{
public:
  PublishReader(const DescriptorType::executor_type &executor, Client &client, InputPrompt &prompt);
  void run();

private:
  void setup_topic_leaf();
  void setup_content();
  void setup_retain();
  void setup_qos();

  void handle_topic_leaf(std::string input);
  void handle_content(std::string input);
  void handle_retain(std::string input);
  void handle_qos(std::string input);

  std::string m_topic_leaf{};
  std::string m_content{};
  boost::mqtt5::retain_e m_retain{};
  boost::mqtt5::qos_e m_qos{};
};