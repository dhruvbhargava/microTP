#include "Server.h"
#include "HttpHeaders.h"
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <string>

inline bool exists(const std::string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
    /* 
  stat function to load the status and meta data if the file exists and returns 1 otherwise returns 0 if the file doesn't exist
  .c_str() used because unix/linux systems use c to post system calls stat system call expects character array and not a string
  */
}
std::string format(std::string http_template, int value)
{
    std::string val = std::to_string(value);
    std::string http_template_end = HTTP_OK_KEEP_ALIVE_END;
    std::string http_string = http_template+val+http_template_end;
    return http_string;
    
}
void HttpServer::GET(const char *filepath, bool close)
{
    std::string root_path = PROJECT_ROOT, file_p = filepath;
    std::string full_path = root_path + file_p + ".html";
    std::cout<<full_path<<std::endl;
    if (!exists(full_path))
    {
        char response[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 12\r\n\r\n<!DOCTYPE html> <html><h1>404 NOT FOUND</h1></html>";
        std::cout << response << std::endl;
        write(new_socket_fd, response, strlen(response));
        return;
    }

    std::ifstream html(full_path);
    std::ostringstream html_stream;
    html_stream << html.rdbuf();
    std::string html_string = html_stream.str();
    std::string http_template = close ? (char *)HTTP_OK_CLOSE_START : HTTP_OK_KEEP_ALIVE_START;
    std::string http_string = format(http_template,html_string.length());
    std::string response = http_string + html_string;
    write(new_socket_fd, response.c_str(), strlen(response.c_str()));
    return;
}