#include "web_server_help.h"


void WebServer::createSocket() {
  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
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
     return processConnection(lst);
   }
 }

 int WebServer::processConnection (int connection) {
   logger->printLog("Connection made");
   std::memset(buffer, 0, sizeof(buffer));
   n = 0;
   if ((n = read(connection, buffer, 255)) < 0) {
     logger->printLog("Error reading data");
     exit(-1);
     return -1;
   }
   else {
     logger->printLog("read() successful");
     std::string fileName = ""; // file requested
     for (int i = 0; i < 256; i++) {
       //std::cout << buffer[i];
       // checking for html files
       if (buffer[i] == 'f' && buffer[i+1] == 'i' && buffer[i+2] == 'l' && buffer[i+3] == 'e') {
         for (int j = 0; j < 10; j++) {
           fileName += buffer[i + j];
         }
       }
       // checking for jpeg files
       if (buffer[i] == 'i' && buffer[i+1] == 'm' && buffer[i+2] == 'a' && buffer[i+3] == 'g' && buffer[i+4] == 'e') {
         for (int j = 0; j < 11; j++) {
           fileName += buffer[i + j];
         }
       }
     }
     // if invalid fileName was typed
     if (fileName == "") {
       for (int i = 0; i < 256; i++) {
         if (buffer[i] == 'G' && buffer[i + 1] == 'E' && buffer[i + 2] == 'T') {
           std::string str =
              "HTTP/1.1 404 Not Found\n"
              "Content-type: text/html\n"
              "Content-Length: 15\n"
              "Accept-Ranges: bytes\n"
              "Connection: close\n"
              "\n"
              "File Not Found\n";
              strcpy(returnBuffer, str.c_str());
              std::cout << strlen(returnBuffer) << std::endl;
              n = send(connection, returnBuffer, strlen(returnBuffer), 0);
              close(connection);
              return 404;          // valid GET request, wrong file name
            }
            else {
              std::string str =
              "HTTP/1.1 400 Bad Response\n"
              "Content-type: text/html\n"
              "Content-Length: 15\n"
              "Accept-Ranges: bytes\n"
              "Connection: close\n"
              "\n"
              "Invalid Request\n";
              strcpy(returnBuffer, str.c_str());
              std::cout << strlen(returnBuffer) << std::endl;
              n = send(connection, returnBuffer, strlen(returnBuffer), 0);
              close(connection);
              return 400;          // not valid GET request, wrong file name
            }
          }
        }
        else {
          // fileName that starts with "file" or "image" was given
        }


     std::cout << fileName << std::endl;
   }
   return 0;
 }
