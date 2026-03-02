#include "client.h"
#include "generator.h"

#include <boost/asio/io_context.hpp>
#include <iostream>

int main()
{
  boost::asio::io_context ioc;
  Generator generator;
  Client client{ioc.get_executor(), generator};
  client.run();

  ioc.run();
}