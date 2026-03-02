#pragma once

#include "reader_base.h"

class LimitsReader : public ReaderBase
{
public:
  LimitsReader(const DescriptorType::executor_type &executor, Client &client, InputPrompt &prompt);
  void run();

private:
  void setup_min();
  void setup_max();

  void handle_min(std::string input);
  void handle_max(std::string input);

  static bool read_value(const std::string &input, int64_t &value);

  int64_t m_min{};
  int64_t m_max{};
};