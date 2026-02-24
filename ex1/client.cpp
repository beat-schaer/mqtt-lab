#include "client.h"

#include "input_prompt.h"

#include <format>
#include <functional>
#include <iostream>

using namespace boost::mqtt5;
using namespace std::placeholders;

Client::Client(const ClientType::executor_type &executor, const InputPrompt &prompt, std::string my_name,
               std::string peer_name)
    : m_mqtt_client{executor}
    , m_prompt{prompt}
    , m_my_name{std::move(my_name)}
    , m_peer_name{std::move(peer_name)}
{
  m_mqtt_client.brokers("broker.mqtt", 1883);
}

void Client::run()
{
  m_mqtt_client.async_run(std::bind(&Client::on_connect, this, _1));
  subscribe_myself();
}

void Client::disconnect()
{
  m_mqtt_client.async_disconnect(disconnect_rc_e::normal_disconnection, {},
                                 std::bind(&Client::on_disconnect, this, _1));
}

void Client::publish_chat(std::string_view topic_leaf, std::string_view content, bool retained, int qos,
                          std::function<void()> finished_cb)
{
  auto topic = std::format("{}/{}", m_peer_name, topic_leaf);
  const auto retain = retained ? retain_e::yes : retain_e::no;
  switch (qos) {
  case 0:
    m_mqtt_client.async_publish<qos_e::at_most_once>(std::move(topic), std::string{content}, retain, {},
                                                     std::bind(&Client::on_puback_0, this, _1, std::move(finished_cb)));
    break;

  case 1:
    m_mqtt_client.async_publish<qos_e::at_least_once>(
        std::move(topic), std::string{content}, retain, {},
        std::bind(&Client::on_puback_12, this, _1, _2, std::move(finished_cb)));
    break;

  case 2:
    m_mqtt_client.async_publish<qos_e::exactly_once>(
        std::move(topic), std::string{content}, retain, {},
        std::bind(&Client::on_puback_12, this, _1, _2, std::move(finished_cb)));
    break;

  default:
    std::cout << "Invalid QoS: " << qos << std::endl;
    finished_cb();
  }
}

void Client::subscribe_myself()
{
  subscribe_topic topic;
  topic.topic_filter = std::format("{}/#", m_my_name);
  m_mqtt_client.async_subscribe(topic, {}, std::bind(&Client::on_suback, this, _1, _2, _3));
}

void Client::setup_receive()
{
  m_mqtt_client.async_receive(std::bind(&Client::on_receive, this, _1, _2, _3, _4));
}

void Client::on_connect(boost::mqtt5::error_code err)
{
  if (err || err == boost::asio::error::operation_aborted) {
    return;
  }
  std::cout << "\nConnection failed! error=" << err.message() << std::endl;
}

void Client::on_disconnect(boost::mqtt5::error_code err)
{
  std::cout << "\nDisconnected! result=" << err.message() << std::endl;
}

void Client::on_suback(boost::mqtt5::error_code err, const std::vector<boost::mqtt5::reason_code> &reason_codes,
                       const boost::mqtt5::suback_props &suback_props)
{
  std::cout << "\nSubscribe acknowledged! result=" << err.message()
            << " reason_code=" << (reason_codes.size() == 1 ? reason_codes[0].message() : std::string{}) << std::endl;
  m_prompt.redisplay();
  setup_receive();
}

void Client::on_puback_0(boost::mqtt5::error_code err, std::function<void()> finished_cb)
{
  std::cout << "\nPublish acknowledged! result=" << err.message() << std::endl;
  m_prompt.redisplay();
  finished_cb();
}

void Client::on_puback_12(boost::mqtt5::error_code err, boost::mqtt5::reason_code reason_code,
                          std::function<void()> finished_cb)
{
  std::cout << "\nPublish acknowledged! result=" << err.message() << "  reason_code=" << reason_code.message()
            << std::endl;
  m_prompt.redisplay();
  finished_cb();
}

void Client::on_receive(boost::mqtt5::error_code err, const std::string &topic, const std::string &content,
                        const boost::mqtt5::publish_props &props)
{
  if (err) {
    if (err != boost::asio::error::operation_aborted) {
      std::cout << "\nReceive failed! error=" << err.message() << std::endl;
      m_prompt.redisplay();
    }
    return;
  }
  std::cout << "\n*** Message received from peer: topic=" << topic << "  content=" << content << " ***" << std::endl;
  m_prompt.redisplay();
  setup_receive();
}
