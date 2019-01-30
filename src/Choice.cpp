#include "Choice.h"

Choice::Choice(const char id, const std::string& text)
:
id(id),
text(text)
{}

Choice::Choice(const std::string& str)
{
    // assumes "(a) text"
    char _id = str[1];                  // TODO validate
    std::string _text = str.substr(4);  // TODO validate
    
    Choice(_id, _text);
}

char Choice::getId(void)
{
    return id;
}

std::string Choice::getText(void)
{
    return text;
}

std::string Choice::serialize(void)
{
    std::stringstream ss;
    ss << "(" << id << ")";
    ss << " " << text << std::endl;
    return ss.str();
}