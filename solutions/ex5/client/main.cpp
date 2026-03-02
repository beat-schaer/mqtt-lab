#include "client.h"
#include "input_prompt.h"
#include "limits_reader.h"

#include <boost/asio/io_context.hpp>
#include <iostream>

int main()
{
  InputPrompt prompt;
  boost::asio::io_context ioc;
  Client client{ioc.get_executor(), prompt};
  client.run();

  LimitsReader limits_reader{ioc.get_executor(), client, prompt};
  limits_reader.run();

  ioc.run();
}