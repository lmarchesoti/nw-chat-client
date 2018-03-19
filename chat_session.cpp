#include "chat_session.h"

#include <mutex>
#include <vector>
#include <string>

#include <iostream>
#include <thread>

#include <unistd.h>

bool ChatSession::start() {

  std::lock_guard<std::mutex> lock(this->global_lock);

  this->conn.connect_to_server(this->host);

  if(!this->conn.validate_username(this->username)) {

    std::cout << "Username already exists" << std::endl;
    return true;
  }

	this->listen = true;
  this->listener = std::thread(&ChatSession::listen_to_server, std::ref(*this));

  return false;
}

std::vector<std::string> ChatSession::get_messages() {

  std::lock_guard<std::mutex> lock(this->global_lock);
  return this->chatbox;
}

void ChatSession::end() {

	this->listen = false;

  std::lock_guard<std::mutex> lock(this->global_lock);
  this->conn.disconnect();

	this->listener.join();
}

void ChatSession::listen_to_server() {

  std::string msg;

  while (this->listen) {

    msg = this->conn.receive();
    
		if (this->listen)
			this->include_msg(msg);

    usleep(1000);
  }
}

void ChatSession::include_msg(std::string msg) {

  std::lock_guard<std::mutex> lock(this->global_lock);

  this->chatbox.push_back(msg);
}

bool ChatSession::is_alive() { 

  std::lock_guard<std::mutex> lock(this->global_lock);

  return this->conn.is_alive();
}

void ChatSession::send_msg(std::string msg) {

  this->conn.send_msg(msg);
}
