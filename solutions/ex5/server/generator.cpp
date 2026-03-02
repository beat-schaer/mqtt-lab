#include "generator.h"

#include <boost/json.hpp>
#include <iostream>

using namespace boost::json;

void Generator::generate_number(const std::string_view args, const ResponseCb &response_cb)
{
  boost::system::error_code ec;
  const auto value = parse(args, ec);
  if (ec) {
    std::cout << "Parsing failed: " << ec.message() << std::endl;
    return;
  }
  const auto *arg_obj = value.if_object();
  if (!arg_obj) {
    std::cout << "Argument should be an object but is not." << std::endl;
    return;
  }
  const auto *lower_element = arg_obj->if_contains("min");
  const auto *upper_element = arg_obj->if_contains("max");
  if (!lower_element || !upper_element) {
    std::cout << "Either lower or upper limit not set." << std::endl;
    return;
  }
  const auto *lower_value = lower_element->if_int64();
  const auto *upper_value = upper_element->if_int64();
  if (!lower_value || !upper_value) {
    std::cout << "Lower and upper element exist but either is not an int64." << std::endl;
    return;
  }

  std::uniform_int_distribution<int64_t> distribution(*lower_value, *upper_value);
  const auto random_number = distribution(m_generator);

  object result_obj;
  result_obj["result"] = random_number;
  response_cb(serialize(result_obj));
}