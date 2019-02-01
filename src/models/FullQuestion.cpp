#include "FullQuestion.h"

FullQuestion::FullQuestion(const std::string& str)
{
    init(str);
}

FullQuestion::FullQuestion(std::stringstream& ss)
{
    init(ss);
}

FullQuestion::FullQuestion(std::vector<std::string> tags,
                           std::string question,
                           std::map<const char, Choice> choices,
                           char solution)
{
    init(tags, question, choices, solution);
}

FullQuestion::FullQuestion(Question question, char solution)
{
    init(question, solution);
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

void FullQuestion::init(std::vector<std::string> tags,
                        std::string question,
                        std::map<const char, Choice> choices,
                        char solution)
{
    Question questionObj(tags, question, choices);
    init(questionObj, solution);
}

void FullQuestion::init(const Question question, const char solution)
{
    validate(question, solution);
    _question = new Question(question);
    _solution = solution;
}   

void FullQuestion::validate(const Question& question,
     const char solution) const
{
    // NOTE: no need to check for solution being an
    //       non [a-z] character, the Choice that would
    //       match with it would fail to instantiate
    //       hence the next check would take care of it
    const std::map<const char, Choice> 
        _choices = question.getAllChoices();

    if (_choices.find(solution) == _choices.end())
        throw std::invalid_argument(
            "Provided solution is not in the set of choices.");
}

char FullQuestion::getSolution(void) const
{
    return _solution;
}

const Question& FullQuestion::getQuestion(void) const
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
    ss << getQuestion().serialize();
    ss << serializeSolution(_solution) << std::endl;
    return ss.str();
}