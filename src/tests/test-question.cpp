#include "Question.h"
#include "Escape.h"
#include <iostream>
#include <ostream>

bool questionProgrammaticallyGoldenCase(void);

void runTest(const std::string& name, bool test(void));

int main(void)
{
    runTest("questionProgrammaticallyGoldenCase", 
        questionProgrammaticallyGoldenCase);

    return 0; 
}

void runTest(const std::string& name, bool test(void))
{
    std::cout << name << ":";
    std::cout << (test() ? "[OK]" : "[FAILED]");
    std::cout << std::endl;
}

bool questionProgrammaticallyGoldenCase(void)
{
    std::string expectedSerialization("presidents, US history\n"
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
        ".\n");

    TagCollection tags("presidents, US history");
    QuestionTitle questionTitle(
        "Which is the first president of the USA\n.\n");
    ChoiceCollection choices("(a) Thomas Jefferson\n"
        ".\n"
        "(b) Abraham Lincoln\n"
        ".\n"
        "(c) George Washington\n"
        ".\n"
        "(d) Benjamin Franklin\n"
        ".\n"
        ".\n");

    Question question(tags, questionTitle, choices);

    if(question.getTags() != tags)
        return false;

    if(question.getQuestionTitle() != questionTitle)
        return false;

    if(question.getTags() != tags)
        return false;

    if(question.serialize() != expectedSerialization)
        return false;

    std::ostringstream os;
    os << question;
    if(os.str() != expectedSerialization)
        return false;

    return true;
}