#include "Socket.h"
#include "Host.h"
#include <iostream>
#include <ostream>
#include <istream>
#include <sstream>

bool socketOpenAndClose(void);
//bool socketWriteAndRead(void);
bool hostCreate(void);
bool socketBindLocalhost(void);
bool testCommunication(void);

void runTest(const std::string& name, bool test(void));

int main(void)
{
    runTest("socketOpenAndClose", socketOpenAndClose);
    runTest("hostCreate", hostCreate);
    runTest("socketBindLocalhost", socketBindLocalhost);
    runTest("testCommunication", testCommunication);

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

bool hostCreate(void)
{
    Host host("localhost", "80");

    if(host.getAddress() != "127.0.0.1")
        return false;

    if(host.getNode() != "localhost")
        return false;

    if(host.getService() != "80")
        return false;
    
    if(host.isPassive() == true)
        return false;    
    
    return true;
}

bool socketBindLocalhost(void)
{
    Socket socket(IPv4, StreamSocket, TCP);
    Host host("8080");

    socket.bindSocket(host);

    if(!socket.isBinded())
        return false;   

    socket.closeSocket();

    if(socket.isBinded())
        return false;

    return true;
}

bool testCommunication(void)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        // child process (client)
        Host host("localhost", "8080");
        Socket socket;
        socket.onOutgoingConnection = [](
            ISocket& socket, 
            const IHost& host, 
            ISocket* context)
        {
            std::cout << "CLIENT: Client connected" << std::endl;

            std::istringstream is("Hello there!");
            socket.writeToSocket(is);

            std::ostringstream os;
            socket.readFromSocket(os);
            std::cout << "CLIENT: server replied -> " << os.str();
        };
        std::cout << "CLIENT: now trying to connect" << std::endl;
        socket.connectTo(host);
    }
    else if (pid > 0)
    {
        // parent process (server)
        Host host("8080");
        Socket socket;
        socket.onIncomingConnection = [](
            ISocket& socket, 
            const IHost& host, 
            ISocket* context)
        {
            std::cout << "SERVER: Server connected" << std::endl;
            
            std::ostringstream os;
            socket.readFromSocket(os);
            std::cout << "SERVER: client said -> " << os.str();
            
            std::istringstream is("Hello back!");
            socket.writeToSocket(is);
        };
        socket.bindSocket(host);
        socket.startListening();
        std::cout << "SERVER: now trying to accept" << std::endl;
        socket.acceptConnection();
    }
    else
    {
        // fork failed
        std::cout << "Fork failed on communication test" << std::endl;
        return false;
    }

    return true;
}