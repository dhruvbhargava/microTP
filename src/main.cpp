#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "Server.h"

int main(int argc, char *argv[])
{
	if (argc<3){
		std::cout<<"no files to host"<<std::endl;
		exit(1);
	}
	char* port,*project_root;
	port = argv[1];
	project_root = argv[2];
	HttpServer server = HttpServer(port,project_root);
	server.startListen();

}