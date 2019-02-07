#include "QuizServer.h"

QuizServer::QuizServer(IQuizBook* quizbook, 
                       IHost* host, 
                       ISocket* socket,
                       const std::string& filename)
{
    _host = host;
    _quizbook = quizbook;
    _socket = socket;
    _filename = filename;

    // before setting handlers,
    // reset quizbook and load with file
    _quizbook->clear();
    std::ifstream ifs(_filename);
    _quizbook->readFrom(ifs);

    // set handlers ------------------------------------------------------

    _socket->onIncomingConnection = [&]
    (ISocket& socket, const IHost& host, ISocket* context) {
        serveRequest(socket, host, context);
    }; // set handler

    _quizbook->onInsert = [&](const ISolvedQuestion& question, 
                              IQuizBook* book)
    {
        changeHandler(question, book);
    };

    _quizbook->onDelete = [&](ISolvedQuestion& question, 
                              IQuizBook* book)
    {
        // this reference to the deleted question will cease to exist
        // after the event is handled, if a copy is needed, it has to
        // be created
        changeHandler(question, book);
    };

    _quizbook->onClear = [&](IQuizBook* book)
    {
        clearHandler(book);
    };

    setRequestHandlers();
}


void QuizServer::changeHandler(const ISolvedQuestion& question, 
                              IQuizBook* book)
{
    std::ofstream file(_filename, std::ios::out);
    book->writeTo(file);
    file.close();
}

void QuizServer::clearHandler(IQuizBook* book)
{
    std::ofstream file(_filename, std::ios::out);
    book->writeTo(file);
    file.close();
}

QuizServer::~QuizServer(){}

void QuizServer::run(void)
{
    _socket->bindSocket(*(_host));
    _socket->startListening();

    _running = true;
    while(_running)
        _socket->acceptConnection();
}

void QuizServer::serveRequest(ISocket& socket, const IHost& host, ISocket* context)
{
    // read type of request
    std::ostringstream os;
    socket.readFromSocket(os, 1); // only one character
    std::string typeStr = os.str();
    char type = typeStr.front();
    // call appropiate handler
    (_reqHandlers.at(type))(socket, host, context); // TODO check if handler exists and handle gracefully
}

void QuizServer::setRequestHandlers(void)
{
    //p -> put a question in the bank
    _reqHandlers.emplace('p', 
    [&](ISocket& socket, const IHost& host, ISocket* context){
        // read question until \n.\n
        std::string buff = "";
        std::string term = "\n.\n";
        size_t tL = term.length();
        
        do
        {
            std::ostringstream os;
            socket.readFromSocket(os);
            buff.append(os.str());
        }while(buff.substr(buff.length() - tL - 1, tL) != term);

        // drop leading end-of-line from option input
        if(buff.front() == '\n') buff = buff.substr(1);

        // try to create question... if if fails... send back message
        std::istringstream is;
        try {
            SolvedQuestion q(buff);
            uint32_t id = _quizbook->insertQuestion(q);
            is = std::istringstream(std::to_string(id));
        } catch (const std::exception& e) {
            is = std::istringstream("Invalid format" 
                                    + std::string(e.what()));
        }

        socket.writeToSocket(is);
    });

    //d -> delete a question from the bank
    _reqHandlers.emplace('d', 
    [&](ISocket& socket, const IHost& host, ISocket* context){
        std::string buff = "";
        do
        {
            std::ostringstream os;
            socket.readFromSocket(os);
            buff.append(os.str());
        }while(buff.back() != '\n');

        buff.erase(buff.find_last_not_of(" \n\r\t")+1);
        buff = buff.substr(buff.find_first_not_of(" \n\r\t"));

        // try to delete question, if it fails, report back
        std::istringstream is;
        try {
            uint32_t id = (uint32_t)std::stoul(buff);
            _quizbook->deleteQuestionById(id);
            is = std::istringstream(std::string("Deleted question ") 
                                    + std::to_string(id));
        } catch (const std::exception& e) {
            is = std::istringstream(std::string("Error: question ") 
                                    + buff + " not found.");
        }

        socket.writeToSocket(is);
    });

    //g -> get a question from the bank
    _reqHandlers.emplace('g', 
    [&](ISocket& socket, const IHost& host, ISocket* context){
        std::string buff = "";
        do
        {
            std::ostringstream os;
            socket.readFromSocket(os);
            buff.append(os.str());
        }while(buff.back() != '\n');

        buff.erase(buff.find_last_not_of(" \n\r\t")+1);
        buff = buff.substr(buff.find_first_not_of(" \n\r\t"));

        // try to get question, if it fails, report back
        std::istringstream is;
        try {
            uint32_t id = (uint32_t)std::stoul(buff);
            SolvedQuestion q = _quizbook->getQuestionById(id);
            is = std::istringstream(q.serialize());
        } catch (const std::exception& e) {
            is = std::istringstream(std::string("Error: question ") 
                                    + buff + " not found.");
        }

        socket.writeToSocket(is);
    });

    //r -> get a random question from the bank
    _reqHandlers.emplace('r', 
    [&](ISocket& socket, const IHost& host, ISocket* context){
        // try to get question, if it fails, report back
        std::istringstream is;
        SolvedQuestion q;
        try {
            uint32_t id;
            q = _quizbook->getRandomQuestion(id);
            is = std::istringstream(
                std::to_string(id) + "\n" + q.getQuestion().serialize());
        } catch (const std::exception& e) {
            is = std::istringstream("Error: There are no questions.");
        }

        socket.writeToSocket(is);

        // read response
        std::ostringstream choiceOss;
        socket.readFromSocket(choiceOss);
        char choice = choiceOss.str().front();
        
        std::string response = "Incorrect";
        if(q.getSolution() == choice)
            response = "Correct";
        
        std::istringstream responseIss(response);
        socket.writeToSocket(responseIss);
    });
}

