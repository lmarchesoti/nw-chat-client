#include "connection.h"

#include <string>
#include <memory>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Connection::~Connection() { close(*sockfd); }

void Connection::connect_to_server(std::string hostname) { 

    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname.c_str(), PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	exit(1);
        //return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if (*(sockfd = std::make_shared<int>(socket(p->ai_family, p->ai_socktype,
                p->ai_protocol))) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(*sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(*sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
	exit(2);
        //return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure
}

std::string Connection::receive() {

  char buf[MAXDATASIZE];
  int numbytes;  

  if ((numbytes = recv(*sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
      perror("recv");
      exit(1);
  }

  buf[numbytes] = '\0';

  return std::string(buf);
}

void Connection::listen_to_server() {

  while (true)
    this->receive_msg();
}

void Connection::receive_msg() {
    msg_queue->push_back(receive());
}

void Connection::send_msg(std::string msg){

  if (send(*sockfd, msg.c_str(), msg.length(), 0) == -1)
      perror("send");

}

bool Connection::validate_username(std::string username) {

  send_msg(username);

  if (receive() == "OK") {

    send_msg("OK");
    return true;
  }

  return false;
}
