#ifndef __QUIZSERVER__H__
#define __QUIZSERVER__H__

#include "Exception.h"
#include "IHost.h"
#include "Socket.h"
#include "IQuizBook.h"

#include <map>
#include <fstream>
#include <functional>

class QuizServer
{
    public:
        
        QuizServer(IQuizBook* quizbook, 
                   IHost* host, 
                   ISocket* socket,
                   const std::string& filename);

        void run(void);

        ~QuizServer(); // dealloc Socket object

    protected:

        IHost* _host;
        IQuizBook* _quizbook;
        ISocket* _socket;
        std::string _filename;
        bool _running = false;

        // request handlers
        std::map<const char, 
        std::function<void(ISocket&, 
                      const IHost&, 
                      ISocket*)>> _reqHandlers;

        void setRequestHandlers(void);
        void serveRequest(ISocket&, const IHost&, ISocket*);
        void changeHandler(const ISolvedQuestion& question, 
                             IQuizBook* book);
        void clearHandler(IQuizBook* book);
};

#endif // __QUIZSERVER__H__