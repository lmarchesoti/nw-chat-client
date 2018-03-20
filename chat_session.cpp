#include "chat_session.h"

#include <mutex>
#include <vector>
#include <string>

#include <iostream>
#include <thread>
#include <chrono>

#include <unistd.h>

bool ChatSession::start() {

  std::lock_guard<std::mutex> lock1(this->global_lock);
  //std::lock_guard<std::mutex> lock2(this->disconnect_command_mutex);

  this->conn.connect_to_server(this->host);

  if(!this->conn.validate_username(this->username)) {

    std::cout << "Username already exists" << std::endl;
    return true;
  }

	this->listen = true;
  this->listener = std::thread(&ChatSession::listen_to_server, std::ref(*this));

	this->display = std::thread(&ChatSession::update_display, std::ref(*this));
	this->display.detach(); // this always fails to join...

	this->sender = std::thread(&ChatSession::process_input, std::ref(*this));

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
	this->sender.join();
	//this->display.join();
}

void ChatSession::listen_to_server() {

  std::string msg;

  while (this->listen) {

    msg = this->conn.receive();
    
		if (this->listen)
			this->include_msg(msg);

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void ChatSession::include_msg(std::string msg) {

  std::lock_guard<std::mutex> lock(this->global_lock);

  this->chatbox.push_back(msg);
}

bool ChatSession::is_alive() { 

  std::lock_guard<std::mutex> lock(this->global_lock);

  return this->conn.is_alive() && !this->disconnect_command;
}

void ChatSession::send_msg(std::string msg) {

  this->conn.send_msg(msg);
}

void ChatSession::update_display() {

	int num_msgs = 0;
	while(this->is_alive()) {

		auto msgs = this->get_messages();
		if (msgs.size() > num_msgs) {

			std::cout << "client: received " << msgs.at(num_msgs) << std::endl;
			++num_msgs;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void ChatSession::process_input() {

	std::string data;

	this->disconnect_command = false;

	while(this->is_alive() && (std::getline(std::cin, data))) {
		this->send_msg("message\n");
		this->send_msg(data+"\n");
	}

	this->disconnect_command = true;

}
