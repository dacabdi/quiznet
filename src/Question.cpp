#include "Question.h"

Question::Question(const std::string& str)
{
    init(str);
}

Question::Question(std::stringstream& ss)
{
    init(ss);
}

Question::Question(uint32_t id, const std::string& str)
{
    init(id, str);
}

Question::Question(uint32_t id, std::stringstream& ss)
{   
    init(id, ss);
}

Question::Question(
    uint32_t id, 
    std::vector<std::string> tags,
    std::string question,
    std::map<const char, Choice> choices,
    char solution)
{
    init(id, tags, question, choices, solution);
}

void Question::init(const std::string& str)
{
    std::stringstream ss(str);
    init(ss);
}

void Question::init(std::stringstream& ss)
{
    uint32_t id = deserializeId(ss);
    init(id, ss);
}

void Question::init(uint32_t id, const std::string& str)
{
    std::stringstream ss(str);
    init(id, ss);
}

void Question::init(int32_t id, std::stringstream& ss)
{
    // temp fields
    std::vector<std::string> tags = deserializeTag(ss);
    std::string question = deserializeQuestion(ss);
    std::map<const char, Choice> choices = deserializeChoices(ss);
    char solution = deserializeSolution(ss);

    init(id, tags, question, choices, solution);
}

void Question::init(uint32_t id, 
    std::vector<std::string> tags,
    std::string question,
    std::map<const char, Choice> choices,
    char solution)
{
    
    validate(choices, solution);

    // if it validated, it is safe to set
    _id = id;
    _tags = tags;
    _question = question;
    _choices = choices;
    _solution = solution;
}

void Question::validate(std::map<const char,Choice> choices,char solution)
{
    if (choices.size() < 2)
        throw std::invalid_argument(
            "Cannot create question with less than 2 choices.");
    
    char choiceId = 'a';
    for(std::map<const char, Choice>::const_iterator it = choices.begin();
        it != choices.end(); ++it)
        if((*it).first != choiceId)
            throw std::invalid_argument(
                "Choices are non consecutive or do not start wit 'a'");
        else
            choiceId++;

    // NOTE: no need to check for solution being an
    //       non [a-z] character, the Choice that would
    //       match with it would fail to instantiate
    //       hence the next check would take care of it
    if (choices.find(solution) == choices.end())
        throw std::invalid_argument(
            "Provided solution is not in the set of choices.");
}

uint32_t Question::deserializeId(std::stringstream& ss) const
{
    std::string buffer;
    std::getline(ss, buffer);
    return deserializeId(buffer);  
}

uint32_t Question::deserializeId(const std::string& s) const
{
    return (uint32_t)std::stoul(s);
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
    std::string line;
    std::stringstream ssbuff();

    // if the first line is '.', no question, malformed message? TODO ASK!
    std::getline(ss, line);
    if(line == ".")
        throw std::invalid_argument("Empty question."); 
    
    buffer.append(line);
    std::getline(ss, line);
    
    while(line != ".")
    {
        buffer.append("\n");
        buffer.append(line);
        std::getline(ss, line);
    }
    
    return deserializeQuestion(buffer);
}

std::string Question::deserializeQuestion(
    const std::string& question) const
{
    // allows for more complex formatting if needed
    return question;
}

std::map<const char, Choice> Question::deserializeChoices(
    std::stringstream& ss) const
{
    std::map<const char, Choice> _choices;
    std::string buffer;
    bool firstPeriod = false, secondPeriod = false;
    
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
    std::string buffer;
    std::getline(ss, buffer);
    return deserializeSolution(buffer);
}

char Question::deserializeSolution(const std::string& s) const
{
    // allows for more complex formatting if needed
    return s.front();
}

uint32_t Question::getId(void) const
{
    return _id;
}

std::vector<std::string> Question::getTags(void) const
{
    return _tags;
}

std::string Question::getQuestion(void) const
{
    return _question;
}

std::map<const char, Choice> Question::getAllChoices(void) const
{
    return _choices;
}

Choice Question::getChoiceById(char id) const
{
    return _choices.at(id);
}

char Question::getSolution(void) const
{
    return _solution;
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

    ss << serializeId(_id) << std::endl;
    ss << serializeTags(_tags) << std::endl;
    ss << serializeQuestion(_question) << std::endl;
    ss << serializeChoices(_choices) << std::endl;
    ss << serializeSolution(_solution) << std::endl;
    
    return ss.str();
}