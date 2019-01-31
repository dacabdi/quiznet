#include "FullQuestion.h"

FullQuestion::FullQuestion(const std::string& str)
{
    init(str);
}

FullQuestion::FullQuestion(std::stringstream& ss)
{
    init(ss);
}

FullQuestion::FullQuestion(uint32_t id, const std::string& str)
{
    init(id, str);
}

FullQuestion::FullQuestion(uint32_t id, std::stringstream& ss)
{   
    init(id, ss);
}

FullQuestion::FullQuestion(
    uint32_t id, 
    std::vector<std::string> tags,
    std::string question,
    std::map<const char, Choice> choices,
    char solution)
{
    init(id, tags, question, choices, solution);
}

FullQuestion::FullQuestion(Question questionObj, char solution)
{
    init(questionObj, solution);
}

FullQuestion::~FullQuestion(void)
{
    delete _question;
}

void FullQuestion::init(const std::string& str)
{
    std::stringstream ss(str);
    init(ss);
}

void FullQuestion::init(std::stringstream& ss)
{
    Question question(ss);
    char solution = deserializeSolution(ss);
    init(question, solution);
}

void FullQuestion::init(const uint32_t id, const std::string& str)
{
    std::stringstream ss(str);
    init(id, ss);
}

void FullQuestion::init(const uint32_t id, std::stringstream& ss)
{
    Question question(id, ss);
    char solution = deserializeSolution(ss);
    init(question, solution);
}

void FullQuestion::init(
    uint32_t id, 
    std::vector<std::string> tags,
    std::string question,
    std::map<const char, Choice> choices,
    char solution)
{
    Question questionObj(id, tags, question, choices);
    init(questionObj, solution);
}

void FullQuestion::init(const Question questionObj, const char solution)
{
    validate(questionObj, solution);
    _question = new Question(questionObj);
    _solution = solution;
}   

void FullQuestion::validate(const Question& questionObj,
     const char solution) const
{
    // NOTE: no need to check for solution being an
    //       non [a-z] character, the Choice that would
    //       match with it would fail to instantiate
    //       hence the next check would take care of it
    const std::map<const char, Choice> 
        _choices = questionObj.getAllChoices();

    if (_choices.find(solution) == _choices.end())
        throw std::invalid_argument(
            "Provided solution is not in the set of choices.");
}

char FullQuestion::getSolution(void) const
{
    return _solution;
}

const Question& FullQuestion::getQuestionObj(void) const
{
    return (*_question);
}

char FullQuestion::deserializeSolution(std::stringstream& ss) const
{
    std::string buffer;
    std::getline(ss, buffer);
    return deserializeSolution(buffer);
}

char FullQuestion::deserializeSolution(const std::string& s) const
{
    // allows for more complex formatting if needed
    return s.front();
}

std::string FullQuestion::serializeSolution(const char solution) const
{
    return std::string(1, solution);
}

std::string FullQuestion::serialize(void) const
{
    std::stringstream ss;
    ss << getQuestionObj().serialize();
    ss << serializeSolution(_solution) << std::endl;
    return ss.str();
}