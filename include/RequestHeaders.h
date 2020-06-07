#pragma once
#include <string>



class Request
{
public:
    std::string request_method;
    std::string url;
    std::string http_version;
    std::string content_type;
    std::string content_encoding;
    std::string content_language;
    std::string content_location;
};

