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


std::string utils::escape(const std::string& ref)
{
    std::ostringstream oss;
    for (const char* char_p = ref.c_str(); *char_p != '\0'; char_p++)
    {
        switch (*char_p)
        {
            case '\a':  oss << "\\a"; break;
            case '\b':  oss << "\\b"; break;
            case '\f':  oss << "\\f"; break;
            case '\n':  oss << "\\n"; break;
            case '\r':  oss << "\\r"; break;
            case '\t':  oss << "\\t"; break;
            case '\v':  oss << "\\v"; break;
            case '\\':  oss << "\\\\"; break;
            case '\'':  oss << "\\'"; break;
            case '\"':  oss << "\\\""; break;
            case '\?':  oss << "\\\?"; break;
            default: oss << *char_p;
        }
    }
        
    return oss.str();
}
