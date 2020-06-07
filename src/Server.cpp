#include "Server.h"
#include <iostream>
#include "ResponseHeaders.h"
#include "Parser.h"
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
    std::cout << "started Listening for requests\n";
    listen(socket_listen_fd, 5); //a waiting queue size of 5 will suffice since each client is being served by a different process
    pid_t pid = 1;
    while (1)
    {
        new_socket_fd = accept(socket_listen_fd, (sockaddr *)&client_addr, &clielen);
        pid = fork();
        if (pid == 0)
        {
            break;
        }
        std::cout << "Parent" << std::endl;
    }
    if (pid == 0)
    {
        while (1)
        {
            std::string requestBuffer = feedBuffer();
            requestResolver(requestBuffer);
        }
        close(new_socket_fd);
    }
}

std::string HttpServer::feedBuffer()
{
    char requestBuffer[62000];
    int index = 0;
    // std::cout << "buffer contains" << std::endl;
    // std::cout<<requestBuffer<<std::endl;
    while (true)
    {
        read(new_socket_fd, requestBuffer + index, 1);
        if (index > 2 && requestBuffer[index] == '\r' && requestBuffer[index - 2] == '\r')
        {

            break;
        }
        index++;
    }
    std::string reqBuff = requestBuffer;
    return reqBuff;
}

void HttpServer::requestResolver(std::string requestBuffer)
{
    std::cout << std::endl;
    Request Header = parse(requestBuffer);
    std::cout << Header.request_method << std::endl;
    if (Header.request_method.compare("GET") == 0)
    {
        const char *req = Header.url.c_str();
        GET(req, false);
    }
}
