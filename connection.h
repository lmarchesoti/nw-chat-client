#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <mutex>

#include <memory>
#include <vector>
#include <string>

class Connection {

public:
  ~Connection();
  void connect_to_server(std::string);
  std::string receive();
  void send_msg(std::string);
  bool validate_username(std::string);

  void receive_msg();
  bool is_alive();
  void disconnect();

private:
  bool socket_ok();

  std::shared_ptr<int> sockfd;

};

#endif
