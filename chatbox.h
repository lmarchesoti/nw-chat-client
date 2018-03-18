#ifndef _CHATBOX_H_
#define _CHATBOX_H_

#include <string>
#include <mutex>
#include <vector>

class Chatbox {

public:
  void add_msg(std::string);
  //std::string operator[];
  std::vector<std::string> all();

private:
  
  std::mutex data_mutex;
  std::vector<std::string> data;
};

#endif
