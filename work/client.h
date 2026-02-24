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

  Client(const ClientType::executor_type &executor, const InputPrompt &prompt, std::string my_name,
         std::string peer_name);

  void run();
  void disconnect();
  void publish_chat(std::string_view topic_leaf, std::string_view content, bool retained, int qos,
                    std::function<void()> finished_cb);

private:
  void subscribe_myself();
  void setup_receive();

  void on_connect(boost::mqtt5::error_code err);
  void on_disconnect(boost::mqtt5::error_code err);
  void on_suback(boost::mqtt5::error_code err, const std::vector<boost::mqtt5::reason_code> &reason_codes,
                 const boost::mqtt5::suback_props &suback_props);
  void on_puback_0(boost::mqtt5::error_code err, std::function<void()> finished_cb);
  void on_puback_12(boost::mqtt5::error_code err, boost::mqtt5::reason_code reason_code,
                    std::function<void()> finished_cb);
  void on_receive(boost::mqtt5::error_code err, const std::string &topic, const std::string &content,
                  const boost::mqtt5::publish_props &props);

  ClientType m_mqtt_client;
  const InputPrompt &m_prompt;
  const std::string m_my_name;
  const std::string m_peer_name;
};