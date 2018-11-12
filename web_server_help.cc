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

 void WebServer::waitingConnection () {
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
     int result = processConnection(lst);
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
         for (int j = 0; j < 10; j++) {
           fileName += buffer[i + j];
         }
       }
     }
     std::cout << "Recieved file name: " << fileName << std::endl;
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
              std::memset(returnBuffer, 0, sizeof(returnBuffer));
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
          std::cout << "Trying to figure out file extension" << std::endl;
          // fileName that starts with "file" or "image" was given
          if (fileName.find("jpg") != std::string::npos) {
            //process jpeg
            std::cout << "processing jpg file" << std::endl;
            FILE *file;
            char *reply;
            int fileSize = 0;
            // Open file
            const char *temp = fileName.c_str();
            file = fopen(temp, "rb");
            logger->printLog("Opening JPG file");
            if (!file)
            {
              std::string str =
                 "HTTP/1.1 404 Not Found\n"
                 "Content-type: text/html\n"
                 "Content-Length: 15\n"
                 "\n"
                 "File Not Found\n";
                 std::memset(returnBuffer, 0, sizeof(returnBuffer));
                 strcpy(returnBuffer, str.c_str());
                 std::cout << strlen(returnBuffer) << std::endl;
                 n = send(connection, returnBuffer, strlen(returnBuffer), 0);
                 close(connection);
                 return 404;          // valid GET request, wrong file name
            }

            // get file Length
            fseek(file, 0, SEEK_END);
            fileSize = ftell(file);
            fseek(file, 0, SEEK_SET);

            //Allocate memory
            char* tempBuffer = (char *)malloc(fileSize + 1);
            if (!tempBuffer)
            {
                fprintf(stderr, "Memory error!");
                fclose(file);
                return -1;
            }

            //Read file contents into buffer
            int resultSize = fread(tempBuffer, fileSize, 1, file);
            fclose(file);

            std::string str =
            "HTTP/1.1 200 OK\n"
            "Content-Type: image/jpeg\n";
            str = str + "Content-Length: " + std::to_string(fileSize);
            logger->printLog(str);

            char test[strlen(str.c_str()) + fileSize + 1];
            std::memset(test, 0, strlen(str.c_str()) + fileSize);

            strcpy(test, str.c_str());
            for (int i = 0; i < fileSize; i++){
              test[(strlen(str.c_str())) + i] = tempBuffer[i];
            }
            strcat(test + strlen(str.c_str()) + fileSize, "\n");

            logger->printLog("Buffer size: " + std::to_string(sizeof(test)));

            while (n > 0) {
              n = send(connection, test, sizeof(test), 0);
              std::cout << std::to_string(n) << std::endl;
              if (n < 0) {
                std::string errorString = strerror(errno);
                logger->printLog("Error with processConnection: " + errorString);
                exit(-1);
              }
            }

            logger->printLog("Done");
            close(connection);
            return connection;

          }
          else if (fileName.find("html") != std::string::npos) {
            //process html
            std::cout << "processed html file" << std::endl;
            FILE *file;
            char *reply;
            int fileSize = 0;
            // Open file
            const char *temp = fileName.c_str();
            file = fopen (temp, "r");

            if (!file)
            {
              std::string str =
                 "HTTP/1.1 404 Not Found\n"
                 "Content-type: text/html\n"
                 "Content-Length: 15\n"
                 "\n"
                 "File Not Found\n";
              std::memset(returnBuffer, 0, sizeof(returnBuffer));
              strcpy(returnBuffer, str.c_str());
              std::cout << strlen(returnBuffer) << std::endl;
              n = send(connection, returnBuffer, strlen(returnBuffer), 0);
              close(connection);
              return 404;          // valid GET request, wrong file name
            }

            // get file Length
            fseek(file, 0, SEEK_END);
            fileSize = ftell(file);
            fseek(file, 0, SEEK_SET);

            //Allocate memory
            char* tempBuffer = (char *)malloc(fileSize + 1);
            if (!tempBuffer)
            {
                fprintf(stderr, "Memory error!");
                fclose(file);
                return -1;
            }

            //Read file contents into buffer
            fread(tempBuffer, fileSize, 1, file);
            fclose(file);

            std::string str =
            "HTTP/1.1 200 OK\n"
            "Content-Type: text/html\n";
            str = str + "Content-Length: " + std::to_string(fileSize) + "\n";

            std::memset(returnBuffer, 0, sizeof(returnBuffer));
            strcpy(returnBuffer, str.c_str());
            strcat(returnBuffer + strlen(str.c_str()), tempBuffer);
            strcat(returnBuffer + strlen(str.c_str()) + strlen(tempBuffer), "\n");

            n = write(connection, returnBuffer, sizeof(returnBuffer));
            close(connection);
            return 0;
          }
          else {
            // file not found
            std::string str =
               "HTTP/1.1 404 Not Found\n"
               "Content-type: text/html\n"
               "Content-Length: 15\n"
               "\n"
               "File Not Found\n";
               strcpy(returnBuffer, str.c_str());
               std::cout << strlen(returnBuffer) << std::endl;
               n = send(connection, returnBuffer, strlen(returnBuffer), 0);
               close(connection);
               return 404;          // valid GET request, wrong file name
          }
        }


     std::cout << fileName << std::endl;
   }
   return 0;
 }
