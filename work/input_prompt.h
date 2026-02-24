#pragma once

#include <iostream>
#include <string>
#include <string_view>

class InputPrompt
{
public:
  void set_and_diplay(std::string_view prompt)
  {
    m_prompt = std::format("{} >", prompt);
    redisplay();
  }
  void clear() { m_prompt.clear(); }

  void redisplay() const { std::cout << m_prompt << std::flush; }

private:
  std::string m_prompt;
};