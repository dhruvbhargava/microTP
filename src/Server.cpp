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
        // std::cout << "CONNN" << std::endl;
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
        char requestBuffer[62000];
        while (1)
        {   
            std::cout << getpid() << std::endl;
            headerParse(requestBuffer);
            requestResolver(requestBuffer);
        }
        close(new_socket_fd);
    }
}

void HttpServer::headerParse(char *requestBuffer)
{
    int index = 0;
    int r_c = 0;
    std::cout<<"buffer contains"<<std::endl;
    // std::cout<<requestBuffer<<std::endl;
    while (true) {
        // std::cout<<"current r count = "<<r_c<<std::endl;
        read(new_socket_fd, requestBuffer+index, 1);  
        // std::cout<<"current index buffer item"<<requestBuffer[index]<<std::endl;
        if (index>2 && requestBuffer[index] ==  '\r' && requestBuffer[index-2]=='\r') {
            // r_c++;
            break;
            // if(r_c == 2) break;
        }
        
        index++;
   }
   std::cout<<"finaa  bruh"<<std::endl;
}

void HttpServer::GETResponse(char *ROOT_PATH)
{
    char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";
    std::cout << response << std::endl;
    write(new_socket_fd, response, strlen(response));
}

void HttpServer::requestResolver(char *requestBuffer)
{
    std::cout << "pid  " <<getpid() << std::endl;
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
