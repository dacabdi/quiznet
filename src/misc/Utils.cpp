#include "Utils.h"

std::vector<std::string> utils::split(
    const std::string& str, 
    const std::string& delimiter)
{
    std::string s = str;
    std::vector<std::string> output;
    size_t pos = 0;
    std::string token;

    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        output.push_back(token);
        s.erase(0, pos + delimiter.length());
    }

    output.push_back(s);

    return output;
}
