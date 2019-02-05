#include "Socket.h"
#include <iostream>
#include <ostream>
#include <istream>
#include <sstream>

bool socketOpenAndClose(void);
//bool socketWriteAndRead(void);

void runTest(const std::string& name, bool test(void));

int main(void)
{
    runTest("socketOpenAndClose", socketOpenAndClose);
    //runTest("socketWriteAndRead", socketWriteAndRead);

    return 0; 
}

void runTest(const std::string& name, bool test(void))
{
    std::cout << name << ":" << std::flush;
    std::cout << (test() ? "[OK]" : "[FAILED]") << std::flush;
    std::cout << std::endl;
}

bool socketOpenAndClose(void)
{
    Socket socket(IPv4, StreamSocket, TCP);

    if(socket.getAddressDomain() != IPv4)
        return false;

    if(socket.getSocketType() != StreamSocket)
        return false;

    if(socket.getProtocol() != TCP)
        return false;

    return true;
}

/*bool socketWriteAndRead(void)
{
    Socket socket(IPv4, StreamSocket, TCP);

    std::istringstream is("this is a test");
    socket.writeToSocket(is);

    std::ostringstream os;
    socket.readFromSocket(os);

    return true;
}*/
