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
        throw std::out_of_range("QuizBook::getRandomQuestion():"
                                "Empty QuizBook.");

    // create a vector with all the keys
    std::vector<const uint32_t> keys = utils::getAllKeys<const uint32_t, 
                                        const SolvedQuestion>(_questions);

    // select an index of the vector randomly and use that key
    UniformRandom<uint32_t> uf(0, keys.size() - 1);
    uint32_t randKeyIdx = uf.generate();
    getQuestionById(keys[randKeyIdx]);
}

uint32_t QuizBook::insertQuestion(const SolvedQuestion question)
{
    // find a free id for the insertion
    uint32_t id = findFreeId();
    return insertQuestion(id, question);
}

uint32_t QuizBook::insertQuestion(uint32_t id, 
    const SolvedQuestion question)
{
    // insert the question with the provided id
    std::pair<std::map<const uint32_t, const SolvedQuestion>::iterator, 
        bool> placement =_questions.emplace(id, question);

    // if insertion did not happen (possibly duplicate id) ...
    if(!placement.second) // ... then except
        throw std::runtime_error(
            "QuizBook::insertQuestion(): Attempted to"
            " insert a duplicate id.");
    
    // if completed then call event handler 
    // with reference to constructed question
    onInsert((*placement.first).second, this);

    return id; 
}   

SolvedQuestion QuizBook::deleteQuestionById(const uint32_t id)
{
    // make a copy for the event handler
    SolvedQuestion deletedQuestion = _questions.at(id);
    
    // delete and check whether the deletion happened
    if(!_questions.erase(id))
        throw std::runtime_error("QuizBook::deleteQuestionById():"
            "Question id does not exist");

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
    for(const std::pair<const uint32_t, 
        const SolvedQuestion>& kv : _questions)
            os << kv.first   // the id
               << std::endl  // and end of line
               << kv.second  // the question
               << std::endl; // a terminating end of line

    os << std::flush;
    return os;
}

std::istream& QuizBook::readFrom(std::istream& is)
{   
    std::string line;

    while(!is.eof())
    {
        std::getline(is, line);
        
        uint32_t id = std::stoul(line);
        SolvedQuestion question(is);
        
        _questions.emplace(id, question);
    }

    return is;
}

// stringify the entire quizbook
std::string QuizBook::serialize(void) const
{
    std::stringstream ss;
}

// number of questions
size_t QuizBook::size(void) const
{
    return _questions.size();
}

// ------------- PROTECTED METHODS ----------------

void QuizBook::init(const std::string& str)
{
    std::istringstream ss(str);
    init(ss);
}

void QuizBook::init(std::istream& is)
{
    readFrom(is);
}

uint32_t QuizBook::findFreeId(const uint32_t start) const
{
    for(uint32_t id = start; id < std::numeric_limits<uint32_t>::max(); ++id)
        if(!hasQuestion(id))
            return id;

    throw std::runtime_error("QuizBook maxed out");
}