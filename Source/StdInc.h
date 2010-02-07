#include <stdio.h>
#include <stdarg.h>

#include <string>
#include "StringUtils.h"
#include "Definitions.h"

#ifdef WIN32
  #include <winsock2.h>
  #include <windows.h>
#else
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <sys/types.h>
  #include <unistd.h>

  typedef int SOCKET;

  #define INVALID_SOCKET (-1)
  #define SOCKET_ERROR (-1)
  #define closesocket close
#endif

using namespace std;
