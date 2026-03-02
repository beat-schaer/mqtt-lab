#include "limits_reader.h"

#include "client.h"
#include "input_prompt.h"

#include <boost/json.hpp>
#include <charconv>
#include <iostream>

LimitsReader::LimitsReader(const DescriptorType::executor_type &executor, Client &client, InputPrompt &prompt)
    : ReaderBase{executor, client, prompt}
{
}

void LimitsReader::run()
{
  // Call first setup method
  // For now we just disconnect
  m_client.disconnect();
}
