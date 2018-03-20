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

		while(chat.is_alive()) {
			sleep(1);
		}

    std::cout << "disconnecting" << std::endl;
    chat.end();
    std::cout << "disconnected" << std::endl;

    return 0;
}
