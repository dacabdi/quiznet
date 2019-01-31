#include "QuizBook.h"
#include <iostream>

// 1.
// given an empty quizbook, when i insert one question, size is equal to 1
// and retrieving the question by its returns an idempotent question



void runTest(const std::string& name, bool test(void));

int main(void)
{
    QuizBook quizbook();

    return 0;
}

void runTest(const std::string& name, bool test(void))
{
    std::cout << name << ":" << std::flush;
    std::cout << (test() ? "[OK]" : "[FAILED]") << std::flush;
    std::cout << std::endl;
}