#include "Server.h"
#include "Httpheaders.h"
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <bits/stdc++.h>

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

HttpServer::HttpServer(char *port, char *project_root)
{
    PORT_NUMBER = atoi(port);
    PROJECT_ROOT = project_root;
    std::cout << PORT_NUMBER << std::endl;
    socket_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_listen_fd < 0)
    {
        std::cout << "couldn't open a socket!"
                  << "\n";
    }
    std::cout << "opened socket \n";
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT_NUMBER);

    if (bind(socket_listen_fd, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cout << "Binding Failed! "
                  << "\n";
        exit(1);
    }
    std::cout << "socket binded!" << std::endl;
}

void HttpServer::startListen()
{
    socklen_t clielen = sizeof(client_addr);
    std::cout << "started Listening at port \n";
    listen(socket_listen_fd, 10); //can't support more than one any ways
    pid_t pid = 1;
    while (1)
    {
        new_socket_fd = accept(socket_listen_fd, (sockaddr *)&client_addr, &clielen);
        pid = fork();
        if (pid == 0)
        {
            // close(socket_listen_fd);
            break;
        }
        // close(new_socket_fd);
        std::cout << "Parent" << std::endl;
    }
    if (pid == 0)
    {
        while(1){
        char requestBuffer[256];
        std::cout << "child" << std::endl;
        read(new_socket_fd, requestBuffer, sizeof(requestBuffer));
        requestResolver(requestBuffer);
        }
    }
}

void HttpServer::GETResponse(char *ROOT_PATH)
{
    char response[] = "HTTP/1.1 200 OK\nConnection: Keep-Alive\nKeep-Alive: timeout=3600, max=200\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    std::cout << response << std::endl;

    write(new_socket_fd, response, strlen(response));
}

void HttpServer::requestResolver(char *requestBuffer)
{
    std::cout << "string buffer contains the following request:" << std::endl;
    std::cout << std::endl;
    std::cout << requestBuffer << std::endl;
    std::stringstream check1(requestBuffer);
    std::string intermediate;
    std::vector<std::string> Header;
    while (std::getline(check1, intermediate, ' '))
    {
        Header.push_back(intermediate);
    }
    std::string Verb = Header[0];

    std::cout << "IN GET" << std::endl;
    GETResponse(PROJECT_ROOT);
}

//kill inactive processes using polling the only way for keep alive sockets

// #define check(expr)       \
//     if (!(expr))          \
//     {                     \
//         perror(#expr);    \
//         kill(0, SIGTERM); \
//     }

// void enable_keepalive(int sock)
// {
//     int yes = 1;
//     check(setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) != -1);

//     int idle = 1;
//     check(setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(int)) != -1);

//     int interval = 1;
//     check(setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(int)) != -1);

//     int maxpkt = 10;
//     check(setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &maxpkt, sizeof(int)) != -1);
// }