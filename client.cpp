/*
** client.cpp
*/

#include <iostream>

#include <thread>

#include <string>
#include <memory>
#include <vector>

#include "connection.h"

int main(int argc, char *argv[])
{

    if (argc != 3) {
	std::cerr << "usage: client hostname username" << std::endl;
        exit(1);
    }

    std::string hostname(argv[1]);
    std::string username(argv[2]);

    auto chatbox = std::make_shared<std::vector<std::string>>();

    Connection conn(chatbox);
    conn.connect_to_server(hostname);

    if(!conn.validate_username(username)) {

      std::cout << "Username already exists" << std::endl;
      return 1;
    }

    std::thread t1(&Connection::listen_to_server, std::ref(conn));

    int num_msgs = 0;
    while(true) {

      if (chatbox->size() > num_msgs) {

	++num_msgs;

	std::cout << "client: received " << chatbox->back() << std::endl;
      }
    }

    t1.join();

    return 0;
}
