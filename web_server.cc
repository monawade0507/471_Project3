#include "web_server.h"

static volatile int keepRunning = 1;

void intHandler (int num) {
     signal(num, SIG_IGN);
     exit(0);
     keepRunning = 0;
}

int main(int argc, char *argv[])
{
  WebServer web;
  int opt = 0;
  while ((opt = getopt(argc, argv, "v")) !=  -1)
  {
  	switch (opt)
  	{
  		case 'v':
  			web.logger->setLogger(true);
  			web.logger->printLog("Verbose Action captured");
  			break;
  		case ':':
  		case '?':
  		default:
  			std::cout<< "Invalid option: " << argv[0] << std::endl;
  			exit(-1);
  	}
  }

  signal(SIGINT, intHandler);
  // Web Server Operations
  while(keepRunning) {
    web.createSocket();
    web.setupAddress();
    // inside the loop for repeat function
    web.bindSocket();
    web.listenSocket();
    // Wait for the connection with the accept call
    web.waitingConnection();
 }

  return 0;
}
