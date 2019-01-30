#include "Question.h"

Question::Question(
    uint32_t id, 
    std::vector<std::string> tags,
    std::string question,
    std::map<char, std::string> choices,
    char solution)
: 
tags(tags), 
question(question), 
choices(choices), 
solution(solution)
{}

Question::Question(uint32_t id, const std::string& str)
{
    std::istringstream iss(str);
    Question(id, iss);
}

Question::Question(uint32_t id, const std::stringstream& ss)
{   
    // temp fields
    std::vector<std::string> _tags;
    std::string _question;
    std::map<char, Choice> _choices;

    _id = id;
    _tags = deserializeTag(ss);
    _question = deserializeQuestion(ss);
    _choices = deserializeChoices(ss);
    _solution = deserializeSolution(ss);

    Question(_id, _tags, _question, _choices_, _solution);
}

std::vector<std::string> Question::deserializeTag(
    const std::istringstream& ss)
{
    std::string buffer;
    std::getline(ss, buffer);
    return deserializeTag(buffer);   
}

std::vector<std::string> Question::deserializeTag(
    const std::string& s)
{
    return split(s);   
}

std::string Question::deserializeQuestion(const std::stringstream& ss)
{
    std::string buffer;
    std::getline(ss, buffer);
    return deserializeQuestion(buffer);
}

std::string Question::deserializeQuestion(const std::string& s)
{
    // allows for more complex formatting if needed
    return question;
}

std::map<char, Choice> Question::deserializeChoices(const std::stringstream& ss)
{
    // TODO validate

    std::map<char, Choice> _choices;
    std::string buffer;

    std::getline(ss, buffer);

    if(!(buffer == "."))
        std::cout << "WRONG FORMAT! (throw exception here)" << std::endl
        //TODO throw some exception, malformed

    bool firstPeriod = true, 
         secondPeriod = false;
    
    while(!secondPeriod)
    {
        std::getline(ss, buffer);
        
        if (buffer == "." && !firstPeriod)
            firstPeriod = true;
        else if (buffer == "." && firstPeriod)
            secondPeriod = true;
        else
        {
            firstPeriod = false;
            
            Choice _choice(buffer);
            char _id = _choice.getId();
            _choices.emplace(_id, _choice);
        }
    }

    return _choices;
}

std::map<char, Choice> Question::deserializeChoices(const std::string s)
{
    std::sstream ss(s);
    return deserializeChoices(ss);
}

char Question::deserializeSolution(const std::stringstream& ss)
{
    // TODO validate

    std::string buffer;
    std::getline(ss, buffer);
    return deserializeSolution(buffer);
}

char Question::deserializeSolution(const std::string& s)
{
    // TODO validate

    // allows for more complex formatting if needed
    return s.front();
}

std::vector<std::string> Question::getTags(void)
{
    return tags;
}

std::string Question::getQuestion(void)
{
    return question;
}

std::map<char, Choice> Question::getAllChoices(void)
{
    return choices;
}

std::string Question::getChoiceById(char id)
{
    return choices.at(id);
}

char getSolution(void)
{
    return solution;
}

std::string Question::serializeTags(const std::vector<std::string>& tags)
{
    std::stringstream ss;

    std::vector<std::string>::iterator it = tags.begin();
    
    // consider no tags
    if (it != tags.end())
        ss << *it;

    for(it; it != tags.end(); ++it)
    {
        // TODO strip spaces and then fix this
        ss << "," << tag;
    }

    return ss.str();
}

std::string Question::serializeQuestion(const std::string& question)
{
    return question;
}

std::string Question::serializeChoices(const map<char, Choice>& choices)
{
    std::stringstream ss;
    ss << "." << std::endl;

    for (std::pair<char, Choice>& _choice : choices) 
    {
        ss << _choice.second.serialize() 
           << std::endl << "." << std::endl;
    }

    ss << ".";

    return ss.str();        
}

std::string Question::serialize(void)
{
    std::stringstream ss;

    ss << id << std::endl;
    ss << serializeTags(tags) << std::endl;
    ss << serializeQuestion(question) << std::endl;
    ss << serializeChoices(choices) << std::endl;
    ss << serializeSolution(solution) << std::endl;
    
    return ss.str();
}