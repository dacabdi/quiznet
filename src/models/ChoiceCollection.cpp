#include "ChoiceCollection.h"

// ------------- CONSTRUCTORS ---------------

ChoiceCollection::ChoiceCollection(const std::string& str)
{
    std::stringstream ss(str);
    init(ss);
}

ChoiceCollection::ChoiceCollection(std::stringstream& ss)
{
    init(ss);
}

// ------------- PUBLIC INTERFACE ---------------

const Choice& ChoiceCollection::getChoiceById(const char id) const
{
    return _choices.at(id);
}

const std::map<const char, const Choice>& 
    ChoiceCollection::getAllChoices(void) const
{
    return _choices;
}

size_t ChoiceCollection::size(void) const
{
    return _choices.size();
}

char ChoiceCollection::lastChoiceLetter(void) const
{
    // no need to check for empty, it is never empty
    return (--_choices.end())->first;
    
}

std::string ChoiceCollection::serialize(void) const
{
    std::stringstream ss;

    for (const std::pair<const char, const Choice>& pair : _choices)
    {
        ss << "(" << pair.first << ") ";
        ss << pair.second.serialize() << std::endl;
        ss << "." << std::endl;
    }

    ss << "." << std::endl;

    return ss.str();
}

// ------------- PRIVATE METHODS ---------------

void ChoiceCollection::init(std::stringstream& ss)
{
    _choices = deserializeAllChoices(ss);
}

std::map<const char, const Choice>
    ChoiceCollection::deserializeAllChoices(std::stringstream& ss) const 
{
    std::map<const char, const Choice> choices;
    bool firstPeriod = false, 
         secondPeriod = false;
    char currentLetter = 'a';
    std::string buffer;
    size_t count = 0;

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

            // validate format and deserialize
            const std::pair<const char, const Choice> pair = 
                deserializeChoice(buffer);

            // check not repeated
            if (choices.find(pair.first) != choices.end())
                throw std::invalid_argument(
                    "Repeated choice letter:" + pair.first);

            if (pair.first != currentLetter)
                throw std::invalid_argument(
                     std::string("Choice letter out of order. ") + 
                     "Expected (" + std::to_string(currentLetter) +
                     ") and received (" +
                      std::to_string(pair.first) + ").");

            // all checks passed
            choices.insert(pair);
            currentLetter++;
            count++;
        }
    }

    if (count < 2)
        throw std::invalid_argument(
            std::string("A collection of choices must have at ") + 
            "least 2 choices, Only " + 
            std::to_string(count) + " were provided.");

    return choices;
}

const std::pair<const char, const Choice> 
    ChoiceCollection::deserializeChoice(const std::string& str) const
{
    if(!validateChoice(str))
        throw std::invalid_argument(
            "Invalid choice format: [" + str + "]");

    const std::pair<const char, const Choice> 
        pair(str.at(1), Choice(str.substr(4)));

    return pair;
}

bool ChoiceCollection::validateChoice(const std::string& str) const
{
    bool valid = true;

    if  (   str.length() < 4 
        ||  str[0] != '('
        ||  str[1] < 'a'
        ||  str[1] > 'z'
        ||  str[2] != ')'
        ||  str[3] != ' ' 
        )   valid = false;

    return valid;
}

// ------------- OPERATORS ---------------

std::ostream& operator<<(std::ostream &os, const ChoiceCollection& choice)
{
    os << choice.serialize();
    return os;
}

bool ChoiceCollection::operator==(const ChoiceCollection& ref) const
{
    return utils::mapsEqual<const char, const Choice>(
        _choices, 
        ref._choices
    ); 
}

bool ChoiceCollection::operator!=(const ChoiceCollection& ref) const
{
    return !operator==(ref);
}