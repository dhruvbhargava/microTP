#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "server.h"
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

class HttpServer{
    public:
        int socket_listen_fd,PORT_NUMBER,new_socket_fd;
        sockaddr_in server_addr,client_addr;
        char* PROJECT_ROOT;
        char client_buffer[256];
        socklen_t clielen;
        
        HttpServer(char* port,char* PROJECT_ROOT);
        void StartListen();
        void GET(char* path);

};