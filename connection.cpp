#include "connection.h"

#include <mutex>

#include <iostream>
#include <exception>

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

Connection::~Connection() {

  close(*sockfd); 
}

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
    std::cerr << "Disconnected from server" << std::endl;
      //perror("recv");
      //exit(1);
  }

  buf[numbytes] = '\0';

  return std::string(buf);

}

void Connection::send_msg(std::string msg){

  if (send(*sockfd, msg.c_str(), msg.length(), 0) == -1)
      //perror("send");
    throw false;

}

bool Connection::validate_username(std::string username) {

  send_msg(username);

  if (receive() == "OK") {

    send_msg("OK");
    return true;
  }

  return false;
}

bool Connection::is_alive() {

  if (this->socket_ok() == false)
    return false;

  try {

    this->send_msg("ping");

  } catch (bool e) {

    return false;
  }

  return true;
}

void Connection::disconnect() {

  shutdown(*sockfd, SHUT_RDWR);
	//close(*sockfd);
}

bool Connection::socket_ok() {

  int error = 0;
  socklen_t len = sizeof (error);
  int retval = getsockopt (*sockfd, SOL_SOCKET, SO_ERROR, &error, &len);

  if (retval != 0) {
      /* there was a problem getting the error code */
      fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
      //return;
    return false;
  }

  if (error != 0) {
      /* socket has a non zero error status */
      fprintf(stderr, "socket error: %s\n", strerror(error));
    return false;
  }

  return true;

}
