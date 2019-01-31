#include "Choice.h"

Choice::Choice(const char id, const std::string& text)
: id(id), text(text)
{}

Choice::Choice(const std::string& str)
{
    // assumes "(a) somestring"
    if(!validate(str))
        throw std::invalid_argument("Malformed choice string.");

    char _id = str[1];                  
    std::string _text = str.substr(4);

    this->id = _id;
    this->text = _text;
}

bool Choice::validate(const std::string& str)
{
    bool valid = true;

    if  (     str[0] != '('
        ||  str[1] < 'a'
        ||  str[1] > 'z'
        ||  str[2] != ')'
        ||  str[3] != ' ' 
        )   valid = false;

    return valid;
}

char Choice::getId(void)
{
    return id;
}

std::string Choice::getText(void)
{
    return text;
}

std::string Choice::serialize(void) const
{
    std::stringstream ss;
    ss << "(" << id << ")";
    ss << " " << text;
    return ss.str();
}

bool Choice::operator==(const Choice& ref) const
{
    return this->id == ref.id && this->text == ref.text;
}

bool Choice::operator!=(const Choice& ref) const
{
    return !operator==(ref);
}