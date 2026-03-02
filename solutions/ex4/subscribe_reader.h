#pragma once

#include "reader_base.h"

#include <boost/mqtt5.hpp>

class SubscribeReader : public ReaderBase
{
public:
  using NextRunner = std::function<void()>;

  SubscribeReader(const DescriptorType::executor_type &executor, Client &client, InputPrompt &prompt,
                  NextRunner next_runner);
  void run();

private:
  void setup_max_qos();
  void setup_no_local();
  void setup_retain_as_published();

  void handle_max_qos(std::string input);
  void handle_no_local(std::string input);
  void handle_retain_as_published(std::string input);

  const NextRunner m_next_runner;

  boost::mqtt5::qos_e m_max_qos{};
  boost::mqtt5::no_local_e m_no_local{};
  boost::mqtt5::retain_as_published_e m_retain_as_published{};
};