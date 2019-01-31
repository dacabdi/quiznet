#include "Question.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include "Escape.h"


void runTest(const std::string& name, bool test(void));
bool compareVectors(const std::vector<std::string>& v1,
                    const std::vector<std::string>& v2);
bool compareMaps(const std::map<const char, Choice>& m1,
                 const std::map<const char, Choice>& m2);

//TESTs

// programmatic
bool createProgrammaticallyGoldenCase(void);
bool createProgrammaticallyWithTwoChoices(void);
bool createProgrammaticallySolutionNotFound(void);
bool createProgrammaticallyNotEnoughChoices(void);
bool createProgrammaticallyNonConsecutiveChoices(void);
bool createProgrammaticallyNonAStartingChoices(void);

// from string
bool createFromStringGoldenCase(void);
bool createFromStringWithTwoChoices(void);
bool createFromStringSolutionNotFound(void);
bool createFromStringNotEnoughChoices(void);
bool createFromStringNonConsecutiveChoices(void);
bool createFromStringNonAStartingChoices(void);


int main(void)
{
    /*const char *body = "23\n"
        "presidents, US history\n"
        "Which is the first president of the USA\n"
        ".\n"
        "(a) Thomas Jefferson\n"
        ".\n"
        "(b) Abraham Lincoln\n"
        ".\n"
        "(c) George Washington\n"
        ".\n"
        "(d) Benjamin Franklin\n"
        ".\n"
        ".\n"
        "c\n";*/

    /*std::string str(body);
    std::cout << str << std::endl;*/

    runTest("createProgrammaticallyGoldenCase", 
            createProgrammaticallyGoldenCase);
    runTest("createProgrammaticallyWithTwoChoices", 
            createProgrammaticallyWithTwoChoices);
    runTest("createProgrammaticallySolutionNotFound", 
            createProgrammaticallySolutionNotFound);
    runTest("createProgrammaticallyNotEnoughChoices", 
            createProgrammaticallyNotEnoughChoices);
    runTest("createProgrammaticallyNonConsecutiveChoices",
            createProgrammaticallyNonConsecutiveChoices);
    runTest("createProgrammaticallyNonAStartingChoices",
            createProgrammaticallyNonAStartingChoices);


    runTest("createFromStringGoldenCase", 
            createFromStringGoldenCase);
    
    runTest("createFromStringWithTwoChoices", 
            createFromStringWithTwoChoices);
    runTest("createFromStringSolutionNotFound", 
            createFromStringSolutionNotFound);
    runTest("createFromStringNotEnoughChoices", 
            createFromStringNotEnoughChoices);
    runTest("createFromStringNonConsecutiveChoices", 
            createFromStringNonConsecutiveChoices);
    runTest("createFromStringNonAStartingChoices", 
            createFromStringNonAStartingChoices);

    return 0;
}

void runTest(const std::string& name, bool test(void))
{
    std::cout << name << ":";
    std::cout << (test() ? "[OK]" : "[FAILED]");
    std::cout << std::endl;
}

bool compareVectors(const std::vector<std::string>& v1,
                    const std::vector<std::string>& v2)
{
    std::vector<std::string>::const_iterator it1 = v1.begin();
    std::vector<std::string>::const_iterator it2 = v2.begin();

    for(; it1 != v1.end() && it2 != v2.end(); ++it1, ++it2)
        if(*it1 != *it2) 
            return false;

    // one of them hasn't gotten to the end
    if(it1 != v1.end() || it2 != v2.end())
        return false;

    return true;
}

bool compareMaps(const std::map<const char, Choice>& m1,
                 const std::map<const char, Choice>& m2)
{
    std::map<const char, Choice>::const_iterator it1 = m1.begin();
    std::map<const char, Choice>::const_iterator it2 = m2.begin();

    for(; it1 != m1.end() && it2 != m2.end(); ++it1, ++it2)
        if((*it1).first != (*it2).first || (*it1).second != (*it2).second)
            return false;

    // one of them hasn't gotten to the end
    if(it1 != m1.end() || it2 != m2.end())
        return false;

    return true;
}

bool createProgrammaticallyGoldenCase(void)
{
    uint32_t id = 23;
    std::vector<std::string> tags({"presidents", " US history"});
    std::string questionPrompt = "Which is the first president of the USA";
    std::map<const char, Choice> choices;
    char solution = 'c';

    choices.emplace('a', Choice("(a) Thomas Jefferson"));
    choices.emplace('b', Choice("(b) Abraham Lincoln"));
    choices.emplace('c', Choice("(c) George Washington"));
    choices.emplace('d', Choice("(d) Benjamin Franklin"));

    Question question(id, tags, questionPrompt, choices, solution);

    if(question.getId() != id)
        return false;
    
    if(!compareVectors(tags, question.getTags()))
        return false;

    if(question.getQuestion() != questionPrompt)
        return false;

    if(!compareMaps(choices, question.getAllChoices()))
        return false;

    if(question.getChoiceById('a') != choices.at('a'))
        return false;

    if(solution != question.getSolution())
        return false;


    return true;
}

bool createProgrammaticallyWithTwoChoices(void)
{
    uint32_t id = 2;
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'a';

    choices.emplace('a', Choice("(a) TCP"));
    choices.emplace('b', Choice("(b) UDP"));

    Question question(id, tags, questionPrompt, choices, solution);

    if(question.getId() != id)
        return false;
    
    if(!compareVectors(tags, question.getTags()))
        return false;

    if(question.getQuestion() != questionPrompt)
        return false;

    if(!compareMaps(choices, question.getAllChoices()))
        return false;

    if(question.getChoiceById('a') != choices.at('a'))
        return false;

    if(solution != question.getSolution())
        return false;


    return true;
}

bool createProgrammaticallySolutionNotFound(void)
{
    uint32_t id = 2;
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'c';

    choices.emplace('a', Choice("(a) TCP"));
    choices.emplace('b', Choice("(b) UDP"));

    try
    {
        Question question(id, tags, questionPrompt, choices, solution);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createProgrammaticallyNotEnoughChoices(void)
{
    uint32_t id = 2;
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'a';

    choices.emplace('a', Choice("(a) TCP"));

    try
    {
        Question question(id, tags, questionPrompt, choices, solution);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createProgrammaticallyNonConsecutiveChoices(void)
{
    uint32_t id = 2;
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'a';

    choices.emplace('a', Choice("(a) TCP"));
    choices.emplace('c', Choice("(c) UDP"));

    try
    {
        Question question(id, tags, questionPrompt, choices, solution);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createProgrammaticallyNonAStartingChoices(void)
{
    uint32_t id = 2;
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'c';

    choices.emplace('b', Choice("(a) TCP"));
    choices.emplace('c', Choice("(b) UDP"));

    try
    {
        Question question(id, tags, questionPrompt, choices, solution);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFromStringGoldenCase(void)
{
    uint32_t id = 23;
    std::vector<std::string> tags({"presidents", " US history"});
    std::string questionPrompt = "Which is the first president of the USA";
    std::map<const char, Choice> choices;
    char solution = 'c';

    choices.emplace('a', Choice("(a) Thomas Jefferson"));
    choices.emplace('b', Choice("(b) Abraham Lincoln"));
    choices.emplace('c', Choice("(c) George Washington"));
    choices.emplace('d', Choice("(d) Benjamin Franklin"));

    const char *body = "23\n"
        "presidents, US history\n"
        "Which is the first president of the USA\n"
        ".\n"
        "(a) Thomas Jefferson\n"
        ".\n"
        "(b) Abraham Lincoln\n"
        ".\n"
        "(c) George Washington\n"
        ".\n"
        "(d) Benjamin Franklin\n"
        ".\n"
        ".\n"
        "c\n";

    Question question(body);

    if(question.getId() != id)
        return false;
    
    if(!compareVectors(tags, question.getTags()))
        return false;

    if(question.getQuestion() != questionPrompt)
        return false;

    if(!compareMaps(choices, question.getAllChoices()))
        return false;

    if(question.getChoiceById('a') != choices.at('a'))
        return false;

    if(solution != question.getSolution())
        return false;

    return true;
}


bool createFromStringWithTwoChoices(void)
{
    uint32_t id = 2;
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'a';

    choices.emplace('a', Choice("(a) TCP"));
    choices.emplace('b', Choice("(b) UDP"));

    const char *body = "2\n"
        "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        "(b) UDP\n"
        ".\n"
        ".\n"
        "a\n";

    Question question(body);

    if(question.getId() != id)
        return false;
    
    if(!compareVectors(tags, question.getTags()))
        return false;

    if(question.getQuestion() != questionPrompt)
        return false;

    if(!compareMaps(choices, question.getAllChoices()))
        return false;

    if(question.getChoiceById('a') != choices.at('a'))
        return false;

    if(solution != question.getSolution())
        return false;


    return true;
}

bool createFromStringSolutionNotFound(void)
{
    const char *body = "2\n"
        "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        "(b) UDP\n"
        ".\n"
        ".\n"
        "c\n";

    try
    {
        Question question(body);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFromStringNotEnoughChoices(void)
{
    const char *body = "2\n"
        "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        ".\n"
        "a\n";

    try
    {
        Question question(body);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFromStringNonConsecutiveChoices(void)
{
    const char *body = "2\n"
        "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        "(c) UDP\n"
        ".\n"
        ".\n"
        "a\n";

    try
    {
        Question question(body);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFromStringNonAStartingChoices(void)
{
    const char *body = "2\n"
        "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        "(c) UDP\n"
        ".\n"
        ".\n"
        "a\n";

    try
    {
        Question question(body);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}