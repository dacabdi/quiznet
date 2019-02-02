#include "QuizBook.h"

// -------------- CONSTRUCTORS -------------

QuizBook::QuizBook(const std::string& str)
{
    init(str);
}

QuizBook::QuizBook(std::istream& is)
{
    init(is);
}

// -------------- PUBLIC INTERFACE -------------

const SolvedQuestion& QuizBook::getQuestionById(uint32_t id) const
{
    return _questions.at(id);
}

const SolvedQuestion& QuizBook::getRandomQuestion(void) const
{
    if(!size())
        throw std::out_of_range(
            "Attempted to retrieve question on empty QuizBook.");

    std::vector<const uint32_t> keys = utils::getAllKeys<const uint32_t, 
                                        const SolvedQuestion>(_questions);

    uint32_t randKeyIdx = UniformRandom<uint32_t> uf(0, keys.size() - 1);

    getQuestionById(keys[randKeyIdx]);
}

uint32_t QuizBook::insertQuestion(const SolvedQuestion fQuestion)
{
    for(uint32_t id = 0; id < std::numeric_limits<uint32_t>::max(); ++id)
        if(!hasQuestion(id))
            return id;

    throw std::runtime_error("QuizBook maxed out");
}

SolvedQuestion QuizBook::deleteQuestionById(const uint32_t id)
{
    SolvedQuestion deletedQuestion = _questions.at(id);
    _questions.erase(id);
    
    // call onDelete handler with a copy of the question
    onDelete(deletedQuestion, this);

    return deletedQuestion; 
}

bool QuizBook::hasQuestion(const uint32_t id) const
{
    return _questions.find(id) != _questions.end();
}

void QuizBook::clear(void)
{
    _questions.clear();
}

std::ostream& QuizBook::writeTo(std::ostream& os) const
{
    
}

std::istream& QuizBook::readFrom(std::istream& is)
{

}

// stringify the entire quizbook
std::string QuizBook::serialize(void) const
{
    std::stringstream ss;
}

// number of questions
size_t QuizBook::size(void) const
{

}

// ------------- PROTECTED METHODS ----------------

void QuizBook::init(const std::string& str)
{
    std::stringstream ss(str);
    init(ss);
}

void QuizBook::init(std::stringstream ss)
{

}

uint32_t QuizBook::findFreeId(const uint32_t start) const
{
    for(uint32_t id = start; id < std::numeric_limits<uint32_t>::max(); ++id)
        if(!hasQuestion(id))
            return id;

    throw std::runtime_error("QuizBook maxed out");
}