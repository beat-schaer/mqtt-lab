#include "client.h"
#include "input_prompt.h"
#include "reader.h"

#include <boost/asio/io_context.hpp>
#include <iostream>

int main()
{
  InputPrompt prompt;
  prompt.set_and_diplay("Enter your name");
  std::string my_name;
  std::cin >> my_name;

  prompt.set_and_diplay("Enter your peer's name");
  std::string peer_name;
  std::cin >> peer_name;

  boost::asio::io_context ioc;
  Client client{ioc.get_executor(), prompt, std::move(my_name), std::move(peer_name)};
  client.run();
  Reader reader{ioc.get_executor(), client, prompt};
  reader.run();

  ioc.run();
}