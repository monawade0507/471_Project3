#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <vector>
#include <sstream>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <cstring>
#include "log.h"

#define ENDL " (" << __FILE__ << ":" << __LINE__ << ")"

class WebServer {
public:
  WebServer() {}
  void createSocket();
  void setupAddress();
  void bindSocket();
  void listenSocket();
  int waitingConnection();
  Log* logger = new Log();
private:
  int sock = -1;
  int lst = -1;
  struct sockaddr_in
  {
    short family;
    u_short port;
    struct in_addr sin_addr;
    char sin_zero[8];
  };
  struct sockaddr_in servAddr;
  struct sockaddr_in clientAddr;
  char buffer[256];
  int portNum = -1;
  int n = 0;

};
