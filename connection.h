#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <memory>
#include <vector>
#include <string>

class Connection {

public:
  Connection(std::shared_ptr<std::vector<std::string>> q) : msg_queue(q) { };
  ~Connection();
  void connect_to_server(std::string);
  std::string receive();
  void send_msg(std::string);
  bool validate_username(std::string);

  void listen_to_server();
  void receive_msg();

private:
  std::shared_ptr<int> sockfd;
  std::shared_ptr<std::vector<std::string>> msg_queue;
};

#endif
