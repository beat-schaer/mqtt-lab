#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/mqtt5/error.hpp>
#include <boost/mqtt5/mqtt_client.hpp>
#include <functional>
#include <string>
#include <string_view>

class InputPrompt;

class Client
{
public:
  using StreamType = boost::asio::ip::tcp::socket;
  using ClientType = boost::mqtt5::mqtt_client<StreamType>;

  Client(const ClientType::executor_type &executor, const InputPrompt &prompt);

  void run();
  void disconnect();
  void subscribe_myself();
  void publish_rng_request(std::string content);

private:
  void setup_receive();

  void on_connect(boost::mqtt5::error_code err);
  void on_disconnect(boost::mqtt5::error_code err);
  void on_suback(boost::mqtt5::error_code err, const std::vector<boost::mqtt5::reason_code> &reason_codes,
                 const boost::mqtt5::suback_props &suback_props);
  void on_puback_12(boost::mqtt5::error_code err, boost::mqtt5::reason_code reason_code);
  void on_receive(boost::mqtt5::error_code err, const std::string &topic, const std::string &content,
                  const boost::mqtt5::publish_props &props);

  static std::string create_response_topic();

  ClientType m_mqtt_client;
  const InputPrompt &m_prompt;
  std::string m_response_topic;
};