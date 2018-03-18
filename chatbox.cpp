#include "chatbox.h"

#include <string>
#include <mutex>
#include <vector>

void Chatbox::add_msg(std::string msg){

  //std::lock_guard<std::mutex> lock(this->data_mutex);
  this->data.push_back(msg);
}

std::vector<std::string> Chatbox::all(){

  //std::lock_guard<std::mutex> lock(this->data_mutex);
  /*
  std::vector<std::string> msgs;

  for(auto it=this->data.begin(); it!=this->data.end(); ++it)
    msgs.push_back(*it);

  return msgs;
  */
  return this->data;
}

