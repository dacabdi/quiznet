#ifndef __ERROR__H__
#define __ERROR__H__

#include <map>
#include <sstream>

#define UNKERR 0
#define NOTFND 1
#define MALQUE 2
#define CHNFND 3
#define UNKREQ 4
#define INVQID 5
#define EMPTYQ 6
#define REQTSZ 7

struct ErrorMessage {
    size_t number;
    std::string symbol;
    std::string message;
    std::string extra;
};

const static std::map<size_t, ErrorMessage> Errors = {
    {0, {0, "UNKERR", "Unkown error", ""}},
    {1, {1, "NOTFND", "Question not found", ""}},
    {2, {2, "MALQUE", "Malformed question body", ""}},
    {3, {3, "CHNFND", "Choice does not exist in question", ""}},
    {4, {4, "UNKREQ", "Server does not know how to handle request type", ""}},
    {5, {5, "INVQID", "Invalid format for question id provided", ""}},
    {6, {6, "EMPTYQ", "Empty quiz book", ""}},
    {7, {7, "REQTSZ", "Request type length must be one character", ""}}
};

#endif