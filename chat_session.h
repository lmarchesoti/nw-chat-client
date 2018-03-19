#ifndef _CHAT_SESSION_H_
#define _CHAT_SESSION_H_

#include <mutex>
#include <vector>
#include <string>
#include <thread>
#include <atomic>

#include "connection.h"

class ChatSession {

public:

  ChatSession(std::string h, std::string uname) : host(h), username(uname) { };
  bool start();
  std::vector<std::string> get_messages();
  void end();
  void send_msg(std::string);
  void listen_to_server();
  bool is_alive();

private:

  void include_msg(std::string);

  std::string host;
  std::string username;
  std::mutex global_lock;
  std::vector<std::string> chatbox;
  Connection conn;
  std::thread listener;
	std::atomic<bool> listen;
};

#endif
