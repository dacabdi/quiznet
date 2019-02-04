#include "QuizBook.h"
#include <iostream>
#include <fstream>

#include <unistd.h>
#define GetCurrentDir 

bool quizBookLoadOneQuestion(void);

void runTest(const std::string& name, bool test(void));

int main(void)
{
    char cCurrentPath[FILENAME_MAX] = "";

    if (!getcwd(cCurrentPath, sizeof(cCurrentPath)))
    {
        return errno;
    }

    std::cout << "Reading test data files from: " 
              << cCurrentPath << std::endl;

    runTest("quizBookLoadOneQuestion", quizBookLoadOneQuestion);

    return 0;
}

void runTest(const std::string& name, bool test(void))
{
    std::cout << name << ":" << std::flush;
    
    if(!test())
        std::cout << "[FAILED]" << std::flush;
    else
        std::cout << "[OK]" << std::flush;

    /*try
    {
        if(!test())
            std::cout << "[FAILED]" << std::flush;
        else
            std::cout << "[OK]" << std::flush;    
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }*/
    
    std::cout << std::endl;
}

bool quizBookLoadOneQuestion(void)
{
    std::ifstream fs("test-quizbook-load-one.data");
    QuizBook qb;
    qb.readFrom(fs);
    fs.close();

    return true;
}