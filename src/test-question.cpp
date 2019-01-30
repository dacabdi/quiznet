#include "Question.h"
#include <iostream>

bool testChoiceValidString1(void);
bool testChoiceValidString2(void);
bool testChoiceInvalidString1(void);
bool testChoiceInvalidString2(void);
bool testChoiceInvalidString3(void);
bool testChoiceInvalidString4(void);
bool testChoiceData(void);

void runTest(const std::string& name, bool test(void));

int main(void)
{
    runTest("testChoiceValidString1", testChoiceValidString1);
    runTest("testChoiceValidString2", testChoiceValidString2);
    runTest("testChoiceInvalidString1", testChoiceInvalidString1);
    runTest("testChoiceInvalidString2", testChoiceInvalidString2);
    runTest("testChoiceInvalidString3", testChoiceInvalidString3);
    runTest("testChoiceInvalidString4", testChoiceInvalidString4);
    runTest("testChoiceData", testChoiceData);

    return 0; 
}

void runTest(const std::string& name, bool test(void))
{
    std::cout << name << ":";
    std::cout << (test() ? "[OK]" : "[FAILED]");
    std::cout << std::endl;
}

bool testChoiceValidString1(void)
{
    Choice choice("(a) This is a choice");

    if (choice.getId() != 'a') return false;
    if (choice.getText() != "This is a choice") return false;
    if (choice.serialize() != "(a) This is a choice") return false;

    return true;
}

bool testChoiceValidString2(void)
{
    Choice choice("(z) This is a choice");

    if (choice.getId() != 'z') return false;
    if (choice.getText() != "This is a choice") return false;
    if (choice.serialize() != "(z) This is a choice") return false;

    return true;
}

bool testChoiceData(void)
{
    Choice choice('a', "This is a choice");

    if (choice.getId() != 'a') return false;
    if (choice.getText() != "This is a choice") return false;
    if (choice.serialize() != "(a) This is a choice") return false;

    return true;
}

bool testChoiceInvalidString1(void)
{
    try
    {
        Choice choice("/a) This is a question");
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool testChoiceInvalidString2(void)
{
    try
    {
        Choice choice("(%) This is a question");
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool testChoiceInvalidString3(void)
{
    try
    {
        Choice choice("(a)This is a question");
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}

bool testChoiceInvalidString4(void)
{
    try
    {
        Choice choice("(a* This is a question");
        return false;
    }
    catch(const std::exception& e)
    {}

    return true;
}
