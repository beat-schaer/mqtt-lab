#pragma once

#include "reader_base.h"

class LimitsReader : public ReaderBase
{
public:
  LimitsReader(const DescriptorType::executor_type &executor, Client &client, InputPrompt &prompt);
  void run();

private:
  // add your setup() and handle() methods here...
};