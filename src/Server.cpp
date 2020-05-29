#include "Server.h"
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

HttpServer::HttpServer(char* port ,char* project_root){
    PORT_NUMBER = atoi(port);
    PROJECT_ROOT = project_root;
    std::cout<<PORT_NUMBER<<std::endl;
    socket_listen_fd = socket(AF_INET,SOCK_STREAM,0);
    if(socket_listen_fd < 0 ){
        std::cout<<"couldn't open a socket!"<<"\n";
    }
    std::cout<<"opened socket \n";
    bzero(( char*) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT_NUMBER);

    
    if(bind(socket_listen_fd,(sockaddr* ) &server_addr,sizeof(server_addr))<0){
        std::cout<<"Binding Failed! "<<"\n";
    }
    std::cout<<"socket binded!"<<std::endl;

}

void HttpServer::StartListen(){
    socklen_t clielen = sizeof(client_addr);
    std::cout<<"started Listening at port \n";
    listen(socket_listen_fd,10);//can't support more than one any ways
    while(1){
        new_socket_fd = accept(socket_listen_fd,(sockaddr *)&client_addr,&clielen);
        std::cout<<"parent::new_socket_fd :="<<new_socket_fd<<std::endl;
        if(fork() == 0){
            std::cout<<"child::new_socket_fd :="<<new_socket_fd<<std::endl;
            int val = 1;
            setsockopt(new_socket_fd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof val);
            std::cout<<"client connected with socket_id :"<<new_socket_fd<<std::endl;    
        }
    }   
}

void HttpServer::GET(char* ROOT_PATH){


}

void HttpServer::VerbResolver(){
    
}
