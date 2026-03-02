#include "client.h"

#include "generator.h"

#include <iostream>

using namespace boost::mqtt5;
using namespace std::placeholders;

Client::Client(const ClientType::executor_type &executor, Generator &generator)
    : m_mqtt_client{executor}
    , m_generator{generator}
{
  m_mqtt_client.brokers("broker.mqtt", 1883);
}

void Client::run()
{
  m_mqtt_client.async_run(std::bind(&Client::on_connect, this, _1));
  setup_receive();
  subscribe_myself();
}

void Client::disconnect()
{
  m_mqtt_client.async_disconnect(disconnect_rc_e::disconnect_with_will_message, {},
                                 std::bind(&Client::on_disconnect, this, _1));
}

void Client::publish_response(std::string response_topic, std::string content)
{
  m_mqtt_client.async_publish<qos_e::at_least_once>(std::move(response_topic), std::move(content), retain_e::no, {},
                                                    std::bind(&Client::on_puback_12, this, _1, _2));
}

void Client::subscribe_myself()
{
  subscribe_topic topic;
  topic.topic_filter = "rng/generate";
  m_mqtt_client.async_subscribe(topic, {}, std::bind(&Client::on_suback, this, _1, _2, _3));
}

void Client::setup_receive()
{
  m_mqtt_client.async_receive(std::bind(&Client::on_receive, this, _1, _2, _3, _4));
}

void Client::on_connect(boost::mqtt5::error_code err)
{
  if (!err || err == boost::asio::error::operation_aborted) {
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
}

void Client::on_puback_12(boost::mqtt5::error_code err, boost::mqtt5::reason_code reason_code)
{
  std::cout << "\nPublish acknowledged! result=" << err.message() << "  reason_code=" << reason_code.message()
            << std::endl;
}

void Client::on_receive(boost::mqtt5::error_code err, const std::string &, const std::string &content,
                        const boost::mqtt5::publish_props &props)
{
  if (err) {
    if (err != boost::asio::error::operation_aborted) {
      std::cout << "\nReceive failed! error=" << err.message() << std::endl;
    }
    return;
  }
  std::cout << "\n*** Message received from requester: content=" << content << " ***" << std::endl;
  const auto response_topic = props[prop::response_topic];
  if (!response_topic.has_value()) {
    std::cout << "Message has no response topic set. Ignoring request!" << std::endl;
    return;
  }
  m_generator.generate_number(content, [this, topic = response_topic.value()](std::string result) {
    publish_response(topic, std::move(result));
  });
  setup_receive();
}
