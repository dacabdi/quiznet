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

    // set quizbook event handlers
    _quizbook->onInsert = [&](const ISolvedQuestion& question, 
                              IQuizBook* book) { onChange(book); };
    _quizbook->onDelete = [&](ISolvedQuestion& question, 
                              IQuizBook* book) { onChange(book); };
    _quizbook->onClear  = [&](IQuizBook* book) { onChange(book); };

    // set each request type handler
    setRequestHandlers();

    // set incoming connection on socket handler
    _socket->onIncomingConnection = [&]
    (ISocket& socket, const IHost& host, ISocket* context) 
    {
        _log(">>> Incoming connection <<<\n");
        _log("From: " + host.getAddress() + "\n");
        
        _log("Awaiting request...\n");
        std::string response = processRequest(socket);
        _log("Sending response... \n");
        socket.writeToSocket(response);
        _log("Done sending response\n");
    };    
}

void QuizServer::setLogger(std::function<void (const std::string&)> logger)
{
    _log = logger;
}

void QuizServer::run(void)
{
    _log("<><><><> Starting QuizNet Server <><><><>\n");
        
    // bind
    _log("Binding... ");
    _socket->bindSocket(*(_host));
    _log("[OK]\n");
    
    // listen
    _log("Listening on port " + _host->getService() + " ... ");
    _socket->startListening();
    _log("[OK]\n");

    _running = true;
    while(_running)
    {
        //_log("Awaiting connection ... \n\n");
        _socket->acceptConnection();
    }
}

void QuizServer::onChange(IQuizBook* book)
{
    // open the file ...
    std::ofstream file(_filename, std::ios::out);
    book->writeTo(file); // ... and write quizbook to it
    file.close();
}

struct Body QuizServer::readBody(const std::string& request)
{
    // finish reading first line (get length)
    // try to read type, if missing space, it will except
    ssize_t length = 0;
    ssize_t pos;
    try {
        pos = request.find('\n');
        length = std::stoul(request.substr(2, pos));
    } catch (const Exception& e) {
        throw ProtocolException(UNKERR, 
            "Failed to read [length] field" + std::string(e.what()));
    }
    
    // read body content
    std::string content = request.substr(pos+1, length);

    return { length, content };
}

char QuizServer::readType(const std::string& request)
{
    // return character
    return request.at(0);
}

struct Request QuizServer::readRequest(ISocket& socket)
{
    _log("Reading request ...\n");
    
    std::string request = socket.readFromSocket();

    // get type
    _log("Type: ");
    char type = readType(request);
    _log(std::to_string(type) + "\n");
    
    // get body
    _log("Content length: ");
    struct Body body = readBody(request);
    _log(std::to_string(body.length) + "\n");
    
    if(_verbose)
        _log("Content: " + utils::escape(body.content) + "\n");

    _log("Done reading request.\n");

    return { type, body };
}

std::string QuizServer::formOkResponse(const std::string& body)
{
    std::ostringstream oss;
    oss << "o ";
    oss << body.length() << std::endl;
    oss << body << std::endl;
    return oss.str();
}

std::string QuizServer::processRequest(ISocket& socket)
{       
    Request req;
    // try to parse request
    try {
        req = readRequest(socket);
    }
    catch(const ProtocolException& e) {
        _log("Failed to parse request!\n");
        if(_verbose){
            _log(  "-=-=- Exception begin -=-=-\n");
            _log(e.what());
            _log("\n-=-=-= Exception end =-=-=-");
        }
        return e.what();
    }

    // try
    _log("Composing response...\n");
    std::string response;
    try {
        response = formOkResponse((_reqHandlers.at(req.type))(req.body));
    } catch (const ProtocolException& e) {
        _log("Failed to compose response!\n");
        if(_verbose){
            _log(  "-=-=- Exception begin -=-=-\n");
            _log(e.what());
            _log("\n-=-=-= Exception end =-=-=-");
        }
        return e.what();
    }

    if(_verbose) _log("Response: " + utils::escape(response) + "\n");
        _log("Done composing response\n");

    return response;
}


void QuizServer::setRequestHandlers(void)
{
    // p -> put a question in the bank
    _reqHandlers.emplace('p', [&](const Body& body)
    {
        std::string response = "";
        try {
            // create question
            SolvedQuestion question(body.content);
            uint32_t id = _quizbook->insertQuestion(question);
            response = std::to_string(id);
        } catch (const Exception& e) {
            throw ProtocolException(MALQUE, e.what());
        } catch (const std::exception& e) {
            throw ProtocolException(UNKERR, 
            "Body: {" + utils::escape(body.content) + "}");
        }
        return response;
    });

    //d -> delete a question from the bank
    _reqHandlers.emplace('d', [&](const Body& body)
    {    
        try {
            uint32_t id = (uint32_t)std::stoul(body.content);
            _quizbook->deleteQuestionById(id);
        } catch (const Exception& e) {
            throw ProtocolException(NOTFND, 
            "Question id provided {" + utils::escape(body.content) + "}");
        } catch (const std::exception& e) {
            throw ProtocolException(UNKERR, 
            "Question id provided {" + utils::escape(body.content) + "}");
        }
        return "";
    });

    //g -> get a question from the bank
    _reqHandlers.emplace('g', [&](const Body& body)
    {   
        std::string reponse = "";
        try {
            uint32_t id = (uint32_t)std::stoul(body.content);
            SolvedQuestion q = _quizbook->getQuestionById(id);
            reponse = q.serialize();
        } catch (const Exception& e) {
            throw ProtocolException(NOTFND, 
            "Question id provided {" + utils::escape(body.content) + "}");
        } catch (const std::exception& e) {
            throw ProtocolException(UNKERR, 
            "Question id provided {" + utils::escape(body.content) + "}");
        }
        return reponse;
    });

    //r -> get a random question from the bank
    _reqHandlers.emplace('r', [&](const Body& body)
    {
        std::string reponse = "";
        try {
            uint32_t id = (uint32_t)std::stoul(body.content);
            SolvedQuestion q = _quizbook->getQuestionById(id);
            reponse = q.serialize();
        } catch (const Exception& e) {
            throw ProtocolException(EMPTYQ);
        } catch (const std::exception& e) {
            throw ProtocolException(UNKERR, 
            "Attempting to retrieve random question");
        }
        return reponse;
    });
}