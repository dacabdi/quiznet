#ifndef __QUIZCLIENT__H__
#define __QUIZCLIENT__H__

#include "Exception.h"
#include "IHost.h"
#include "ISocket.h"
#include "Socket.h"
#include "Question.h"
#include "SolvedQuestion.h"
#include "Request.h"
#include "Utils.h"

#include <string>
#include <iostream>
#include <limits>
#include <sstream>


class QuizClient
{
    public:

        QuizClient(IHost *host, bool persistent = true,
                   std::istream& input = std::cin, 
                   std::ostream& output = std::cout, 
                   std::ostream& error = std::cerr);

        void run();

    protected:

        std::istream& _input;
        std::ostream& _output;
        std::ostream& _error;

        bool _persistent;
        bool _running;

        IHost *_host = nullptr;
        ISocket *_socket = nullptr;
        QuizClient* _qc = nullptr;

        std::map<const char, 
        std::function<Request (const std::string&)>> _prepareRequest;
        std::function<void (const Response&)> _handleResponse;

        void doRequest(char option, const std::string&);
        void promptLoop(void);

        Response sendAndReceive(Request& request);    
        Response parseResponse(ISocket& socket);
        void init(void);
};

#endif // __QUIZCLIENT__H__