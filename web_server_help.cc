#include "web_server_help.h"


void WebServer::createSocket() {
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    logger->printLog("Error opening socket");
    exit(-1);
  }
  else {
    logger->printLog("Socket was created");
    logger->printLog("Socket Info: " + std::to_string(sock));
  }
}

void WebServer::setupAddress () {
  // define the struct
  srand(time(NULL));
  portNum = (rand() % 10000 + 1024);

  // zero the whole struct; to start with an empty struct
  bzero((char *)&servAddr, sizeof(servAddr));

  // Populate the struct with the information needed for the host's address
  servAddr.family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.port = htons(portNum);
  logger->printLog("Address had been created for socket");
}

void WebServer::bindSocket() {
  int bindSuccess = 0;
  int attempts = 0;
  std::string errorString;
  if (bind(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
    errorString = strerror(errno);Log* logger = new Log();
    logger->printLog("bind() failed: " + errorString);
    exit(-1);
  }
  else {
    logger->printLog("bind() successful. Value returned: " + std::to_string(sock));
    std::cout << "Port Number: " << portNum << std::endl;
  }
}

void WebServer::listenSocket () {
  int listenSocket = 1;
  std::string errorString;
  if (listen(sock, listenSocket) < 0) {
    errorString = strerror(errno);
    logger->printLog("listen() failed: " + errorString);
    exit(-1);
  }
  else {
    logger->printLog("listen() successful. Function returns: " + std::to_string(sock));
  }
}

 int WebServer::waitingConnection () {
   logger->printLog("Attempting accept()");
   socklen_t sizeClient = sizeof(clientAddr);
   lst = accept(sock, (struct sockaddr *) NULL, NULL);
   if (lst < 0) {
     std::string errorString = strerror(errno);
     logger->printLog("Error with accept(): " + errorString);
     exit(-1);
   }
   else {
     logger->printLog("accept() successful");
     return 0;
   }
 }
