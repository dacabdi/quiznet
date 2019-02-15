#ifndef __QUIZSERVER__H__
#define __QUIZSERVER__H__

#include "Exception.h"
#include "Utils.h"
#include "Error.h"
#include "Escape.h"
#include "IHost.h"
#include "Socket.h"
#include "IQuizBook.h"
#include "Request.h"

#include <map>
#include <fstream>
#include <functional>
#include <string>


class QuizServer
{
    public:
        
        QuizServer(IQuizBook* quizbook, 
                   IHost* host, 
                   ISocket* socket,
                   const std::string& filename,
                   bool persistent = true);

        void setLogger(std::function<void (const std::string&)> logger);
        void setLoggerVerbose(bool verbose = true);

        void run(void);

        ~QuizServer() {};

        const static std::map<size_t, ErrorMessage> errors;

    protected:

        IHost* _host;
        IQuizBook* _quizbook;
        ISocket* _socket;

        
        
        std::string _filename;
        bool _running = false;
        
        bool _persistent;           // persistent connection mode
        bool _clientAlive = false;  // client alive for persistent conn 

        // request handlers
        std::map<const char,
        std::function<Response (const Request&)>> _handlers;
        
        std::function<void (const std::string&)> _log = 
        [&](const std::string& str) {};
        bool _verbose = false;

        void exchange(ISocket& socket);
        void persistentSession(ISocket &socket);

        void setRequestHandlers(void);
        void onChange(IQuizBook* book);

        Response attendRequest(ISocket& socket);
        Request readRequest(ISocket& socket);
};

#endif // __QUIZSERVER__H__