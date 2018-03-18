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

#include "chat_session.h"

int main(int argc, char *argv[])
{

    if (argc != 3) {
	std::cerr << "usage: client hostname username" << std::endl;
        exit(1);
    }

    std::string hostname(argv[1]);
    std::string username(argv[2]);

    ChatSession chat(hostname, username);
    if(chat.start() == true)
      return 1;

    int num_msgs = 0;
    while(chat.is_alive()) {

      system("clear");
      auto msgs = chat.get_messages();

      for(auto it=msgs.begin(); it!=msgs.end(); ++it)
	std::cout << "client: received " << *it << std::endl;
      std::cout << "------------" << std::endl;

      sleep(3);
      std::cout << "disconnecting" << std::endl;
      chat.end();
      std::cout << "disconnected" << std::endl;
    }

    return 0;
}
