#ifndef __QUIZSERVER__H__
#define __QUIZSERVER__H__

#include "Exception.h"
#include "IHost.h"
#include "Socket.h"
#include "IQuizBook.h"

class QuizServer
{
    public:
        
        QuizServer(IQuizBook* quizbook, IHost* host, ISocket* socket);
        void run(void);

        ~QuizServer(); // dealloc Socket object

    protected:

        IHost* _host;
        IQuizBook* _quizbook;
        ISocket* _socket;

        void serveRequest(ISocket&, const IHost&, ISocket*);
};

#endif // __QUIZSERVER__H__