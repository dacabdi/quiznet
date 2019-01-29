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
        << inet_ntoa(clientAddress.sin_addr) 
        << std::endl;
    logTerm(log.str());

    // create a buffer to read the message
    char *buffer = new char[bufferSize];
    ssize_t bytesRead = readConn(conn, buffer);

    log.clear();
    log << "\tLength: " << bytesRead << std::endl
        << "\tContent: " << EscapedString{ buffer } << std::endl;
    logTerm(log.str());

    // TODO error management

    std::string response = reply(buffer);

    send(conn, response);

    // TODO error management

    close(conn);
}

ssize_t EchoServer::readConn(const int& conn, char * buffer)
{
    // read into buffer
    logTerm("\tReading message... ");
    bzero(buffer, bufferSize);
    ssize_t bytesRead = read(conn, buffer, bufferSize - 1);
    logTerm("[OK]\n");

    return bytesRead;

    // TODO error management
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
    write(conn, msgCstr, msgLength);
    logTerm(" [OK]\n");
}