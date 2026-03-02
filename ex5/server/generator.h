#pragma once

#include <functional>
#include <random>
#include <string>
#include <string_view>

class Generator
{
public:
  using ResponseCb = std::function<void(std::string result)>;
  void generate_number(const std::string_view args, const ResponseCb &response_cb);

private:
  std::mt19937_64 m_generator;
};