#include "QuizServer.h"
#include "QuizBook.h"

#include <iostream>
#include <ostream>
#include <istream>
#include <sstream>
#include <sys/wait.h>
#include <chrono>
#include <thread>

void runTest(const std::string& name, bool test(void));

int main(void)
{   
    QuizBook qb;
    Host host("8080");
    Socket socket(IPv4, StreamSocket, TCP);
    QuizServer server(&qb, &host, &socket);

    return 0; 
}

void runTest(const std::string& name, bool test(void))
{
    std::cout << name << ":" << std::flush;
    std::cout << (test() ? "[OK]" : "[FAILED]") << std::flush;
    std::cout << std::endl;
}