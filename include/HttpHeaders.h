#define HTTP_OK_KEEP_ALIVE_START "HTTP/1.1 200 OK\r\nContent-Length:"
#define HTTP_OK_KEEP_ALIVE_END "\r\nContent-Type: text/html\r\n\r\n"
#define HTTP_OK_CLOSE_START "HTTP/1.1 200 OK\r\nContent-Length:"
#define HTTP_OK_CLOSE_END "\r\nConnection: Closed\r\nContent-Type: text/html\r\n\r\n"
#define HTTP_NOT_FOUND_START  "HTTP/1.1 404\r\nContent-Length:"
#define HTTP_NOT_FOUND_END "\r\nConnection: Closed\r\nContent-Type: text/html\r\n\r\n"

