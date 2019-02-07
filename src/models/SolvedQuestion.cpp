#include "SolvedQuestion.h"

// ------------- CONSTRUCTORS -------------

SolvedQuestion::SolvedQuestion(const std::string& s)
{
    init(s);
}

SolvedQuestion::SolvedQuestion(std::istream& is)
{
    init(is);
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
    std::ostringstream oss;
    oss << _question << _solution << std::flush;
    return oss.str();
}

// -------------- PROTECTED METHODS -----------

void SolvedQuestion::init(const std::string& str)
{
    std::istringstream iss(str);
    init(iss);
}

void SolvedQuestion::init(std::istream& is)
{
    Question question(is);
    char solution = deserializeSolution(is);

    init(question, solution);
}

void SolvedQuestion::init(Question question, char solution)
{
    if(!question.getChoices().hasChoice(solution))
        throw std::invalid_argument(std::string("SolvedQuestion::init():")
            + "Provided solution is not in the question: " + solution); 

    _question = question;
    _solution = solution;
}

char SolvedQuestion::deserializeSolution(std::istream& is) const
{
    // allows for more complex formatting if needed
    char solution;
    is.get(solution);
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