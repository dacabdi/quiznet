#include "Question.h"

Question::Question(
    uint32_t id, 
    std::vector<std::string> tags,
    std::string question,
    std::map<const char, Choice> choices,
    char solution)
:
tags(tags), 
question(question), 
choices(choices), 
solution(solution)
{}

Question::Question(uint32_t id, const std::string& str)
{
    std::stringstream ss(str);
    Question(id, ss);
}

Question::Question(uint32_t id, std::stringstream& ss)
{   
    // temp fields
    uint32_t _id;
    std::vector<std::string> _tags;
    std::string _question;
    std::map<const char, Choice> _choices;
    char _solution;

    _id = id;
    _tags = deserializeTag(ss);
    _question = deserializeQuestion(ss);
    _choices = deserializeChoices(ss);
    _solution = deserializeSolution(ss);

    Question(_id, _tags, _question, _choices, _solution);
}

std::vector<std::string> Question::deserializeTag(std::stringstream& ss) const
{
    std::string buffer;
    std::getline(ss, buffer);
    return deserializeTag(buffer);   
}

std::vector<std::string> Question::deserializeTag(
    const std::string& s) const
{
    return utils::split(s);   
}

std::string Question::deserializeQuestion(std::stringstream& ss) const
{
    std::string buffer;
    std::getline(ss, buffer);
    return deserializeQuestion(buffer);
}

std::string Question::deserializeQuestion(const std::string& s) const
{
    // allows for more complex formatting if needed
    return question;
}

std::map<const char, Choice> Question::deserializeChoices(
    std::stringstream& ss) const
{
    // TODO validate

    std::map<const char, Choice> _choices;
    std::string buffer;

    std::getline(ss, buffer);

    if(!(buffer == "."))
        throw std::invalid_argument("Choices missing initial period.");

    bool firstPeriod = true, secondPeriod = false;
    
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

std::map<const char, Choice> Question::deserializeChoices(
    const std::string& s) const
{
    std::stringstream ss(s);
    return deserializeChoices(ss);
}

char Question::deserializeSolution(std::stringstream& ss) const
{
    // TODO validate

    std::string buffer;
    std::getline(ss, buffer);
    return deserializeSolution(buffer);
}

char Question::deserializeSolution(const std::string& s) const
{
    // TODO validate

    // allows for more complex formatting if needed
    return s.front();
}

std::vector<std::string> Question::getTags(void) const
{
    return tags;
}

std::string Question::getQuestion(void) const
{
    return question;
}

std::map<const char, Choice> Question::getAllChoices(void) const
{
    return choices;
}

Choice Question::getChoiceById(char id) const
{
    return choices.at(id);
}

char Question::getSolution(void) const
{
    return solution;
}

std::string Question::serializeId(const uint32_t id) const
{
    return std::to_string(id);
}

std::string Question::serializeTags(const std::vector<std::string>& tags) const
{
    std::stringstream ss;

    std::vector<std::string>::const_iterator it = tags.begin();
    
    // consider no tags
    if (it != tags.end())
        ss << *it;

    for(; it != tags.end(); ++it)
    {
        // TODO strip spaces and then fix this
        ss << "," << *it;
    }

    return ss.str();
}

std::string Question::serializeQuestion(const std::string& question) const
{
    return question;
}

std::string Question::serializeChoices(const std::map<const char, Choice>& choices) const
{
    std::stringstream ss;
    ss << "." << std::endl;

    for (const std::pair<const char, Choice>& kv : choices) 
    {
        const Choice& _choice = kv.second;
        ss << _choice.serialize()
           << std::endl << "." << std::endl;
    }

    ss << ".";

    return ss.str();        
}

std::string Question::serializeSolution(const char solution) const
{
    return std::string(1, solution);
}

std::string Question::serialize(void) const
{
    std::stringstream ss;

    ss << serializeId(id) << std::endl;
    ss << serializeTags(tags) << std::endl;
    ss << serializeQuestion(question) << std::endl;
    ss << serializeChoices(choices) << std::endl;
    ss << serializeSolution(solution) << std::endl;
    
    return ss.str();
}