#ifndef __REQUEST__H__
#define __REQUEST__H__

#include <string>

struct Body {
    ssize_t length;
    std::string content;
};

struct Response {
    char type;
    struct Body body;
};

struct Request {
    char type;
    struct Body body;
};

#endif