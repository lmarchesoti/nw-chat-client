#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "chatbox.h"

#include <memory>
#include <vector>
#include <string>

class Connection {

public:
  Connection(std::shared_ptr<Chatbox> q) : msg_queue(q) { };
  ~Connection();
  void connect_to_server(std::string);
  std::string receive();
  void send_msg(std::string);
  bool validate_username(std::string);

  void listen_to_server();
  void receive_msg();
  bool is_alive();
  void disconnect();

private:
  std::shared_ptr<int> sockfd;
  std::shared_ptr<Chatbox> msg_queue;
};

#endif
