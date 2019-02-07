#ifndef __QUIZCLIENT__H__
#define __QUIZCLIENT__H__

#include "Exception.h"
#include "IHost.h"
#include "ISocket.h"
#include "Socket.h"
#include "Question.h"
#include "SolvedQuestion.h"

#include <string>
#include <iostream>
#include <limits>
#include <sstream>

class QuizClient
{
    public:

        QuizClient(IHost *host);
        void run(std::istream& is = std::cin, 
                 std::ostream& os = std::cout,
                 std::ostream& errs = std::cerr);

    protected:

        IHost *_host;
        ISocket *_socket;

        std::map<const char, 
        std::function<void(char option,
                           std::istream&, 
                           std::ostream&,
                           std::ostream&,
                           QuizClient*)>> _handlers;

        void initHandlers(void);
};

#endif // __QUIZCLIENT__H__