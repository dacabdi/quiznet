#include "QuizServer.h"

QuizServer::QuizServer(IQuizBook* quizbook, IHost* host, ISocket* socket)
{
    _host = host;
    _quizbook = quizbook;
    _socket = socket;

    _socket->onIncomingConnection = [&]
    (ISocket& socket, const IHost& host, ISocket* context) {
        serveRequest(socket, host, context);
    }; // set handler
}

QuizServer::~QuizServer(){}

void QuizServer::run(void)
{
    _socket->startListening();
    _socket->bindSocket(*_host);
    _socket->acceptConnection();
}

void QuizServer::serveRequest(ISocket& socket, const IHost& host, ISocket* context)
{
    // do nothing for now TODO
}