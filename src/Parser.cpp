#include "Parser.h"
#include <bits/stdc++.h>
#include <iostream>

std::vector<std::string> split(std::string input, char split_token)
{
    std::stringstream inp(input);
    std::string temp;
    std::vector<std::string> tokenized_out;
    while (std::getline(inp, temp, split_token))
    {
        tokenized_out.push_back(temp);
    }
    return tokenized_out;
}

std::string stripLower(std::string str)
{
    //strips spaces
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    //converts every thing to lower case
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

Request parse(std::string raw_header)
{
    //split the header line by line
    std::vector<std::string> lines = split(raw_header, '\r');
    Request httpRequest =  Request();
    //set the method parameter (@required part)
    std::vector<std::string> method_meta = split(lines[0], ' ');
    httpRequest.request_method = method_meta[0];
    httpRequest.url = method_meta[1];
    httpRequest.http_version = method_meta[2];
    //set the optional fields
    for (int index = 1; index < lines.size(); index++)
    {
        std::string curr = stripLower(lines[index]);
        std::vector<std::string> field_dp = split(curr, ':');
        if (field_dp[0].compare("content-type") == 0)
        {
            httpRequest.content_type = field_dp[1];
        }
        if (field_dp[0].compare("content-encoding") == 0)
        {
            httpRequest.content_encoding = field_dp[1];
        }
        if (field_dp[0].compare("content-language") == 0)
        {
            httpRequest.content_language = field_dp[1];
        }
        if (field_dp[0].compare("content-location") == 0)
        {
            httpRequest.content_location = field_dp[1];
        }
    }
    return httpRequest;
}
