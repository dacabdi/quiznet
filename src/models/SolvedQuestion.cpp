#include "SolvedQuestion.h"

// ------------- CONSTRUCTORS -------------

SolvedQuestion::SolvedQuestion(const std::string& s)
{
    init(s);
}

SolvedQuestion::SolvedQuestion(std::stringstream& ss)
{
    init(ss);
}

SolvedQuestion::SolvedQuestion(Question question, char solution)
{
    init(question, solution);
}

// ---------- PUBLIC INTERFACE ------------

char SolvedQuestion::getSolution(void) const
{
    return _solution;
}

const Question& SolvedQuestion::getQuestion(void) const
{
    return _question;
}

std::string SolvedQuestion::serialize(void) const
{
    std::stringstream ss;
    ss << _question << _solution << std::flush;
    return ss.str();
}

// -------------- PROTECTED METHODS -----------

void SolvedQuestion::init(const std::string& s)
{
    std::stringstream ss(s);
    init(ss);
}

void SolvedQuestion::init(std::stringstream& ss)
{
    Question question(ss);
    char solution = deserializeSolution(ss);

    init(question, solution);
}

void SolvedQuestion::init(Question question, char solution)
{
    if(!question.getChoices().hasChoice(solution))
        throw std::invalid_argument(
            "Provided solution is not in the question: " + solution); 

    _question = question;
    _solution = solution;
}

char SolvedQuestion::deserializeSolution(std::stringstream& ss) const
{
    // allows for more complex formatting if needed
    char solution;
    ss.get(solution);
    return solution;
}

// --------------- OPERATORS -----------------

bool SolvedQuestion::operator==(const SolvedQuestion& ref) const
{
    return (_question == ref._question
        &&  _solution == ref._solution );
}

bool SolvedQuestion::operator!=(const SolvedQuestion& ref) const
{
    return !operator==(ref);
}

std::ostream& operator<<(std::ostream& os, const SolvedQuestion& ref)
{
    os << ref.serialize();
    return os;
}