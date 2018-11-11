#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include "web_server_help.h"

#define ENDL " (" << __FILE__ << ":" << __LINE__ << ")"
