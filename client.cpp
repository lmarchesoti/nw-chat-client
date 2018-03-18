/*
** client.cpp
*/

#include <stdlib.h>

#include <unistd.h>

#include <iostream>

#include <thread>

#include <string>
#include <memory>
#include <vector>

#include "chatbox.h"
#include "connection.h"

int main(int argc, char *argv[])
{

    if (argc != 3) {
	std::cerr << "usage: client hostname username" << std::endl;
        exit(1);
    }

    std::string hostname(argv[1]);
    std::string username(argv[2]);

    //auto chatbox = std::make_shared<std::vector<std::string>>();
    auto chatbox = std::make_shared<Chatbox>();

    Connection conn(chatbox);
    conn.connect_to_server(hostname);

    if(!conn.validate_username(username)) {

      std::cout << "Username already exists" << std::endl;
      return 1;
    }

    std::thread t1(&Connection::listen_to_server, std::ref(conn));

    int num_msgs = 0;
    while(conn.is_alive()) {
//num_msgs == 0 || chatbox->size() > num_msgs) {
//conn.is_alive() || chatbox->size() > num_msgs) {
      system("clear");
      auto msgs = chatbox->all();
      for(auto it=msgs.begin(); it!=msgs.end(); ++it)
	std::cout << "client: received " << *it << std::endl;
      std::cout << "------------" << std::endl;

      sleep(3);
/*
      if (chatbox->size() > num_msgs) {

	std::cout << "client: received " << (*chatbox)[num_msgs] << std::endl;

	++num_msgs;
*/
	
	//conn.disconnect();
      //}
    }

    //t1.join();

    return 0;
}
