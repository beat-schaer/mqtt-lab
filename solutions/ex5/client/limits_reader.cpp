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
  setup_min();
}

void LimitsReader::setup_min()
{
  setup("Enter minimal value for random number (empty to disconnect)", &LimitsReader::setup_min,
        &LimitsReader::handle_min);
}

void LimitsReader::setup_max()
{
  setup("Enter minimal value for random number", &LimitsReader::setup_max, &LimitsReader::handle_max);
}

void LimitsReader::handle_min(std::string input)
{
  if (input.empty()) {
    m_client.disconnect();
    return;
  }
  if (!read_value(input, m_min)) {
    setup_min();
    return;
  }
  setup_max();
}

void LimitsReader::handle_max(std::string input)
{
  if (!read_value(input, m_max)) {
    setup_max();
    return;
  }

  boost::json::object req_obj;
  req_obj["min"] = m_min;
  req_obj["max"] = m_max;
  m_client.publish_rng_request(boost::json::serialize(req_obj));

  setup_min();
}

bool LimitsReader::read_value(const std::string &input, int64_t &value)
{
  const auto [_, ec] = std::from_chars(input.data(), input.data() + input.size(), value);
  const auto error_code = std::make_error_code(ec);
  if (error_code) {
    std::cout << error_code.message() << std::endl;
    return false;
  }
  return true;
}
