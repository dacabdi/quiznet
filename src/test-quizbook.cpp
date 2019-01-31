#include "Quizbook.h"
#include <iostream>

void runTest(const std::string& name, bool test(void));

int main(void)
{
    return 0;
}

void runTest(const std::string& name, bool test(void))
{
    std::cout << name << ":" << std::flush;
    std::cout << (test() ? "[OK]" : "[FAILED]") << std::flush;
    std::cout << std::endl;
}