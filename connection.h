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
  void receive();

private:
  int sockfd;
  std::shared_ptr<std::vector<std::string>> msg_queue;
};

#endif
