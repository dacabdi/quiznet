#include "QuestionTitle.h"

// ----------- CONSTRUCTORS ----------------

QuestionTitle::QuestionTitle(const std::string& title)
{
    _title = deserialize(title);
}

QuestionTitle::QuestionTitle(std::stringstream& ss)
{
    _title = deserialize(ss);
}

// ----------- PUBLIC INTERFACE ----------------

const std::string& QuestionTitle::getText(void) const
{
    return _title;
}

std::string QuestionTitle::serialize(void) const
{
    std::stringstream ss;
    ss << _title << std::endl << "." << std::endl;
    return ss.str();
}

// ---------- OPERATORS -------------------

std::ostream& operator<<(std::ostream &os, const QuestionTitle& question)
{
    os << question.serialize();
    return os;
}

bool QuestionTitle::operator==(const QuestionTitle& ref)
{
    return _title == ref._title;
}

bool QuestionTitle::operator!=(const QuestionTitle& ref)
{
    return !operator==(ref);
}

// ----------- PRIVATE METHODS ----------------

std::string QuestionTitle::deserialize(std::stringstream& ss)
{
    std::string title;
    std::string line;

    // if the first line is '.', no question, malformed message? TODO ASK!
    std::getline(ss, line);
    if(line == ".")
        throw std::invalid_argument("Empty question title."); 
    
    title.append(line);
    std::getline(ss, line);
    
    while(line != ".")
    {
        title.append("\n");
        title.append(line);
        std::getline(ss, line);
    }
    
    return title;
}

std::string QuestionTitle::deserialize(const std::string& str)
{
    size_t pos = str.find("\n.\n");
    if(pos == std::string::npos)
        throw std::invalid_argument(
            "Title format does not end on \\n.\\n");

    return str.substr(0, pos);
}