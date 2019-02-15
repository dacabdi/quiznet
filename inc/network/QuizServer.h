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
                   const std::string& filename);

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

        // request handlers
        std::map<const char,
        std::function<std::string (const Body&)>> _reqHandlers;
        
        std::function<void (const std::string&)> _log = 
        [&](const std::string& str) {};
        bool _verbose = false;

        void setRequestHandlers(void);
        void onChange(IQuizBook* book);

        std::string processRequest(ISocket& socket);
        struct Request readRequest(ISocket& socket);
        struct Body readBody(const std::string&);
        char readType(const std::string&);
        
        std::string formOkResponse(const std::string& body = "");
};

#endif // __QUIZSERVER__H__