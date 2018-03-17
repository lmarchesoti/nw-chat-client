/*
** client.c -- a stream socket client demo
*/

#include <string>
#include <memory>
#include <vector>

#include "connection.h"

int main(int argc, char *argv[])
{

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    std::string hostname(argv[1]);

    auto chatbox = std::make_shared<std::vector<std::string>>();;

    Connection conn(chatbox);
    conn.connect_to_server(hostname);

    conn.receive();

    printf("client: received '%s'\n", chatbox->front().c_str());

    return 0;
}
