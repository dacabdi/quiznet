#include "FullQuestion.h"
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
bool createFullQuestionProgrammaticallyGoldenCase(void);
bool createFullQuestionProgrammaticallyWithTwoChoices(void);
bool createFullQuestionProgrammaticallySolutionNotFound(void);
bool createFullQuestionProgrammaticallyNotEnoughChoices(void);
bool createFullQuestionProgrammaticallyNonConsecutiveChoices(void);
bool createFullQuestionProgrammaticallyNonAStartingChoices(void);

// from string
bool createFullQuestionFromStringGoldenCase(void);
bool createFullQuestionFromStringWithTwoChoices(void);
bool createFullQuestionFromStringSolutionNotFound(void);
bool createFullQuestionFromStringNotEnoughChoices(void);
bool createFullQuestionFromStringNonConsecutiveChoices(void);
bool createFullQuestionFromStringNonAStartingChoices(void);

// from stream
bool createFullQuestionFromStreamGoldenCase(void);
bool createFullQuestionFromStreamWithTwoChoices(void);
bool createFullQuestionFromStreamSolutionNotFound(void);
bool createFullQuestionFromStreamNotEnoughChoices(void);
bool createFullQuestionFromStreamNonConsecutiveChoices(void);
bool createFullQuestionFromStreamNonAStartingChoices(void);



int main(void)
{
    runTest("createFullQuestionProgrammaticallyGoldenCase", 
            createFullQuestionProgrammaticallyGoldenCase);
    runTest("createFullQuestionProgrammaticallyWithTwoChoices", 
            createFullQuestionProgrammaticallyWithTwoChoices);
    runTest("createFullQuestionProgrammaticallySolutionNotFound", 
            createFullQuestionProgrammaticallySolutionNotFound);
    runTest("createFullQuestionProgrammaticallyNotEnoughChoices", 
            createFullQuestionProgrammaticallyNotEnoughChoices);
    runTest("createFullQuestionProgrammaticallyNonConsecutiveChoices",
            createFullQuestionProgrammaticallyNonConsecutiveChoices);
    runTest("createFullQuestionProgrammaticallyNonAStartingChoices",
            createFullQuestionProgrammaticallyNonAStartingChoices);
    
    runTest("createFullQuestionFromStringGoldenCase", 
            createFullQuestionFromStringGoldenCase);
    runTest("createFullQuestionFromStringWithTwoChoices", 
            createFullQuestionFromStringWithTwoChoices);
    runTest("createFullQuestionFromStringSolutionNotFound", 
            createFullQuestionFromStringSolutionNotFound);
    runTest("createFullQuestionFromStringNotEnoughChoices", 
            createFullQuestionFromStringNotEnoughChoices);
    runTest("createFullQuestionFromStringNonConsecutiveChoices", 
            createFullQuestionFromStringNonConsecutiveChoices);
    runTest("createFullQuestionFromStringNonAStartingChoices", 
            createFullQuestionFromStringNonAStartingChoices);

    runTest("createFullQuestionFromStreamGoldenCase", 
            createFullQuestionFromStreamGoldenCase);
    runTest("createFullQuestionFromStreamgWithTwoChoices", 
            createFullQuestionFromStreamWithTwoChoices);
    runTest("createFullQuestionFromStreamSolutionNotFound", 
            createFullQuestionFromStreamSolutionNotFound);
    runTest("createFullQuestionFromStreamNotEnoughChoices", 
            createFullQuestionFromStreamNotEnoughChoices);
    runTest("createFullQuestionFromStreamNonConsecutiveChoices", 
            createFullQuestionFromStreamNonConsecutiveChoices);
    runTest("createFullQuestionFromStreamNonAStartingChoices", 
            createFullQuestionFromStreamNonAStartingChoices);

    return 0;
}

void runTest(const std::string& name, bool test(void))
{
    std::cout << name << ":" << std::flush;
    std::cout << (test() ? "[OK]" : "[FAILED]") << std::flush;
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

bool createFullQuestionProgrammaticallyGoldenCase(void)
{
    std::vector<std::string> tags({"presidents", " US history"});
    std::string questionPrompt = "Which is the first president of the USA";
    std::map<const char, Choice> choices;
    char solution = 'c';

    choices.emplace('a', Choice("(a) Thomas Jefferson"));
    choices.emplace('b', Choice("(b) Abraham Lincoln"));
    choices.emplace('c', Choice("(c) George Washington"));
    choices.emplace('d', Choice("(d) Benjamin Franklin"));

    const char * expectedSerialization = "presidents, US history\n"
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

    const char * expectedSerializationQuestion = "presidents, US history\n"
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
        ".\n";

    FullQuestion fullQuestion(tags, questionPrompt, choices, solution);
    Question question = fullQuestion.getQuestion();

    if(!compareVectors(tags, question.getTags()))
        return false;

    if(question.getQuestion() != questionPrompt)
        return false;

    if(!compareMaps(choices, question.getAllChoices()))
        return false;

    if(question.getChoiceById('a') != choices.at('a'))
        return false;

    if(solution != fullQuestion.getSolution())
        return false;

    if(expectedSerialization != fullQuestion.serialize())
        return false;

    if(expectedSerializationQuestion != question.serialize())
        return false;

    return true;
}

bool createFullQuestionProgrammaticallyWithTwoChoices(void)
{
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'a';

    choices.emplace('a', Choice("(a) TCP"));
    choices.emplace('b', Choice("(b) UDP"));

    FullQuestion fullQuestion(tags, questionPrompt, choices, solution);
    Question question = fullQuestion.getQuestion();
    
    if(!compareVectors(tags, question.getTags()))
        return false;

    if(question.getQuestion() != questionPrompt)
        return false;

    if(!compareMaps(choices, question.getAllChoices()))
        return false;

    if(question.getChoiceById('a') != choices.at('a'))
        return false;

    if(solution != fullQuestion.getSolution())
        return false;


    return true;
}

bool createFullQuestionProgrammaticallySolutionNotFound(void)
{
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'c';

    choices.emplace('a', Choice("(a) TCP"));
    choices.emplace('b', Choice("(b) UDP"));

    try
    {
        FullQuestion question(tags, questionPrompt, choices, solution);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFullQuestionProgrammaticallyNotEnoughChoices(void)
{
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'a';

    choices.emplace('a', Choice("(a) TCP"));

    try
    {
        FullQuestion question(tags, questionPrompt, choices, solution);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFullQuestionProgrammaticallyNonConsecutiveChoices(void)
{
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'a';

    choices.emplace('a', Choice("(a) TCP"));
    choices.emplace('c', Choice("(c) UDP"));

    try
    {
        FullQuestion question(tags, questionPrompt, choices, solution);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFullQuestionProgrammaticallyNonAStartingChoices(void)
{
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'c';

    choices.emplace('b', Choice("(a) TCP"));
    choices.emplace('c', Choice("(b) UDP"));

    try
    {
        FullQuestion question(tags, questionPrompt, choices, solution);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFullQuestionFromStringGoldenCase(void)
{
    std::vector<std::string> tags({"presidents", " US history"});
    std::string questionPrompt = "Which is the first president of the USA";
    std::map<const char, Choice> choices;
    char solution = 'c';

    choices.emplace('a', Choice("(a) Thomas Jefferson"));
    choices.emplace('b', Choice("(b) Abraham Lincoln"));
    choices.emplace('c', Choice("(c) George Washington"));
    choices.emplace('d', Choice("(d) Benjamin Franklin"));

    const char *body = "presidents, US history\n"
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

    const char *bodyQuestion = "presidents, US history\n"
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
        ".\n";

    FullQuestion fullQuestion(body);
    Question question = fullQuestion.getQuestion();
    
    if(!compareVectors(tags, question.getTags()))
        return false;

    if(question.getQuestion() != questionPrompt)
        return false;

    if(!compareMaps(choices, question.getAllChoices()))
        return false;

    if(question.getChoiceById('a') != choices.at('a'))
        return false;

    if(solution != fullQuestion.getSolution())
        return false;

    if(body != fullQuestion.serialize())
        return false;

    if(bodyQuestion != question.serialize())
        return false;


    return true;
}


bool createFullQuestionFromStringWithTwoChoices(void)
{
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'a';

    choices.emplace('a', Choice("(a) TCP"));
    choices.emplace('b', Choice("(b) UDP"));

    const char *body = "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        "(b) UDP\n"
        ".\n"
        ".\n"
        "a\n";

    FullQuestion fullQuestion(body);
    Question question = fullQuestion.getQuestion();
    
    if(!compareVectors(tags, question.getTags()))
        return false;

    if(question.getQuestion() != questionPrompt)
        return false;

    if(!compareMaps(choices, question.getAllChoices()))
        return false;

    if(question.getChoiceById('a') != choices.at('a'))
        return false;

    if(solution != fullQuestion.getSolution())
        return false;


    return true;
}

bool createFullQuestionFromStringSolutionNotFound(void)
{
    const char *body = "network,protocols,transport,delivery\n"
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
        FullQuestion question(body);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFullQuestionFromStringNotEnoughChoices(void)
{
    const char *body = "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        ".\n"
        "a\n";

    try
    {
        FullQuestion question(body);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFullQuestionFromStringNonConsecutiveChoices(void)
{
    const char *body = "network,protocols,transport,delivery\n"
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
        FullQuestion question(body);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFullQuestionFromStringNonAStartingChoices(void)
{
    const char *body = "network,protocols,transport,delivery\n"
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
        FullQuestion question(body);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFullQuestionFromStreamGoldenCase(void)
{
    std::vector<std::string> tags({"presidents", " US history"});
    std::string questionPrompt = "Which is the first president of the USA";
    std::map<const char, Choice> choices;
    char solution = 'c';

    choices.emplace('a', Choice("(a) Thomas Jefferson"));
    choices.emplace('b', Choice("(b) Abraham Lincoln"));
    choices.emplace('c', Choice("(c) George Washington"));
    choices.emplace('d', Choice("(d) Benjamin Franklin"));

    const char *body = "presidents, US history\n"
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

    std::stringstream ss(body);

    FullQuestion fullQuestion(ss);
    Question question = fullQuestion.getQuestion();
    
    if(!compareVectors(tags, question.getTags()))
        return false;

    if(question.getQuestion() != questionPrompt)
        return false;

    if(!compareMaps(choices, question.getAllChoices()))
        return false;

    if(question.getChoiceById('a') != choices.at('a'))
        return false;

    if(solution != fullQuestion.getSolution())
        return false;

    return true;
}


bool createFullQuestionFromStreamWithTwoChoices(void)
{
    std::vector<std::string> tags({"network", "protocols", 
                                   "transport", "delivery"});
    std::string questionPrompt = "Which protocol guarantees delivery?";
    std::map<const char, Choice> choices;
    char solution = 'a';

    choices.emplace('a', Choice("(a) TCP"));
    choices.emplace('b', Choice("(b) UDP"));

    const char *body = "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        "(b) UDP\n"
        ".\n"
        ".\n"
        "a\n";

    std::stringstream ss(body);

    FullQuestion fullQuestion(ss);
    Question question = fullQuestion.getQuestion();
    
    if(!compareVectors(tags, question.getTags()))
        return false;

    if(question.getQuestion() != questionPrompt)
        return false;

    if(!compareMaps(choices, question.getAllChoices()))
        return false;

    if(question.getChoiceById('a') != choices.at('a'))
        return false;

    if(solution != fullQuestion.getSolution())
        return false;


    return true;
}

bool createFullQuestionFromStreamSolutionNotFound(void)
{
    const char *body = "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        "(b) UDP\n"
        ".\n"
        ".\n"
        "c\n";

    std::stringstream ss(body);

    try
    {
        FullQuestion fullQuestion(ss);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFullQuestionFromStreamNotEnoughChoices(void)
{
    const char *body = "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        ".\n"
        "a\n";

    std::stringstream ss(body);

    try
    {
        FullQuestion fullQuestion(ss);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFullQuestionFromStreamNonConsecutiveChoices(void)
{
    const char *body = "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        "(c) UDP\n"
        ".\n"
        ".\n"
        "a\n";

    std::stringstream ss(body);

    try
    {
        FullQuestion fullQuestion(ss);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool createFullQuestionFromStreamNonAStartingChoices(void)
{
    const char *body = "network,protocols,transport,delivery\n"
        "Which protocol guarantees delivery?\n"
        ".\n"
        "(a) TCP\n"
        ".\n"
        "(c) UDP\n"
        ".\n"
        ".\n"
        "a\n";

    std::stringstream ss(body);

    try
    {
        FullQuestion fullQuestion(ss);
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}