#include "web_server.h"

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

  // Web Server Operations
  web.createSocket();
  web.setupAddress();
  web.bindSocket();
  web.listenSocket();
  // Wait for the connection with the accept call
  int returnCode = web.waitingConnection();
  // if returnCode == 404 -> valid GET; invalid file fileName
  if (returnCode == 404) {
    std::cout << "Error 404" << std::endl;
  }
  // if returnCode == 400 -> invalid GET
  if (returnCode == 400) {
    std::cout << "Error 400" << std::endl;
  }

  return 0;
}
