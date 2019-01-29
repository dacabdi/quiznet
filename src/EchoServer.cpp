#include "EchoServer.h"

EchoServer::EchoServer(const std::string userPort)
{

}

void EchoServer::onConnection(const int& conn, 
const sockaddr_in& clientAddress)
{
    // log incoming connection
    std::stringstream log;
    log << "\tIncoming connection from " 
        << inet_ntoa(clientAddress.sin_addr) << std::endl;
    logTerm(log.str());

    // create a buffer to read the message
    char *buffer = new char[bufferSize];
    ssize_t bytesRead = readConn(conn, buffer);
    std::string response = reply(buffer);
    send(conn, response);

    close(conn);
}

ssize_t EchoServer::readConn(const int& conn, char * buffer)
{
    // read into buffer
    logTerm("\tReading message... ");
    bzero(buffer, bufferSize);
    ssize_t bytesRead = read(conn, buffer, bufferSize - 1);
    
    if (bytesRead < 0)
    {
        close(conn);
        error("Could not read from connection");
    }

    logTerm("[OK]\n");

    std::stringstream log;
    log << "\tLength: " << bytesRead << std::endl
        << "\tContent: " << EscapedString{ buffer } << std::endl;
    logTerm(log.str());
    
    return bytesRead;
}

std::string EchoServer::reply(char * msg)
{
    logTerm("\tForming reply... ");
    logTerm("[OK]\n"); // TODO fix this
    return msg;
}

void EchoServer::send(const int& conn, const std::string& msg)
{
    logTerm("\tSending response... ");
    const char * msgCstr = msg.c_str();
    const size_t msgLength = msg.length();

    // write to connection file descriptor
    ssize_t bytesWritten = write(conn, msgCstr, msgLength);

    if(bytesWritten < 0)
    {
        close(conn);
        error("Could not write to connection");
    }

    logTerm(" [OK]\n");
}