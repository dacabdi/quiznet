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
        IHost *_host = nullptr;
        ISocket *_socket = nullptr;
        QuizClient* _qc = nullptr;

        std::map<const char, 
        std::function<std::string (void)>> _prepareRequest;
        std::function<void (const struct Response&)> _handleResponse;

        void doRequest(char option);
        struct Response sendAndReceive(const std::string& request);    
        struct Response parseResponse(ISocket& socket);
        void init(void);
};

#endif // __QUIZCLIENT__H__