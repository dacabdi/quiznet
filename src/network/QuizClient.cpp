#include "QuizClient.h"

QuizClient::QuizClient(
IHost *host, 
bool persistent,
std::istream& input, 
std::ostream& output, 
std::ostream& error)
: _input(input), _output(output), _error(error)
{
    // set server host
    _host = host;

    // use persistent connections or
    // use a new connection per request
    _persistent = persistent;

    init();
}

void QuizClient::promptLoop(void)
{
    char option = '\0';
    std::string prompt = "> ";
    std::string invalidMsg = "Invalid option, please try again.";

    while(_running)
    {
        _output << prompt << std::flush;
        
        std::string line = "";
        std::getline(_input, line);
        
        ssize_t pos = line.find_first_not_of(" \t\n\r");
        if(pos == std::string::npos)
            continue;

        line = line.substr(pos);
        option = line.at(0);

        // special options that do not require requests
        if(option == 'q' && !_persistent)
            _running = false;
        else if (option == 'h') 
            displayMenu();
        else if(!_prepareRequest.count(option))
            _error << invalidMsg << std::endl;
        else
        // options that require requests
            doRequest(option, line);
    }
}

void QuizClient::run(void)
{
    // if connection mode is persistent, then open transport layer already
    if(_persistent)
    {
        // TODO declare to server that session is persistent
        //      do some sort of handshaking

        try
        {
            _output << "Using persistent connection to server." << std::endl;
            _socket = new Socket(IPv4, StreamSocket, TCP);
            _socket->connectTo(*_host);
            _output << "Connected to " << _host->getAddress() 
                    << ":" << _host->getService() << std::endl;    
        } catch(const std::exception& e) {
            _output << "Failed to connect to server: " << std::flush;
            _error << e.what() << std::flush;
            _output << "Stopping client!" << std::endl;
            return;
        }
    }
    
    _running = true;

    promptLoop();
}

void QuizClient::doRequest(char option , const std::string& line)
{
    // compose request
    Request request;
    try {
        request = (_prepareRequest.at(option)(line));
    } catch (const std::exception& e) {
        _error << "Invalid input: " << std::string(e.what()) << std::endl;
        return;
    }

    // send request
    Response response; 
    try {
        response = sendAndReceive(request);
    } catch (const std::exception& e) {
        _error << "Failed to send or receive request: " 
               << std::string(e.what()) << std::endl;
        return;
    }

    (_handleResponse)(response);
}

Response QuizClient::sendAndReceive(Request& request)
{
    Response response;

    if(_persistent)
    {
        // use existing socket
        _socket->writeToSocket(request.serialize());
        response = parseResponse(*_socket);
    }
    else
    {
        // create new socket and connect
        Socket socket(IPv4, StreamSocket, TCP);
        socket.onOutgoingConnection = [&](
        ISocket& socket, const IHost& host, ISocket* context){
            socket.writeToSocket(request.serialize());
            response = parseResponse(socket);
        };
        // try to connect
        socket.connectTo(*_host);
    }

    return response;
}

Response QuizClient::parseResponse(ISocket& socket)
{
    // read response
    std::string buffer = socket.readFromSocket();
    return Response(buffer);
}


void QuizClient::init(void)
{
    _prepareRequest.emplace('p', [&](const std::string& line)
    {
        // build the message
        SolvedQuestion sq(_input);
        Request request('p', sq.serialize());

        // set handler
        _handleResponse = [&](const Response& res) {
            switch (res.getType()) {
                case 'e' : _error   << "Server error!" 
                                    << res.getBody().getContent();
                break;
                case 'o' : _output << res.getBody().getContent();
                break;
            }
            _output << std::endl;
        };

        return request;
    });

    _prepareRequest.emplace('g', [&](const std::string& line)
    {
        // build the message
        Request request('g', utils::trim(line.substr(1)));

        // set handler
        _handleResponse = [&](const Response& res) {
            ErrorMessage err;
            std::string content = res.getBody().getContent();
            switch (res.getType()) 
            {
                case 'e' :  err = utils::deserializeError(content);
                            if (err.number == NOTFND)
                                _error << "Question not found. "
                                       << err.extra << std::endl;
                            else
                                _error << "Server error!" 
                                       << content;
                break;
                case 'o' : _output << content;
                break;
            }
        };

        return request;
    });

    _prepareRequest.emplace('d', [&](const std::string& line)
    {
        // build the message
        Request request('d', utils::trim(line.substr(1)));

        // set handler
        _handleResponse = [&](const Response& res) {
            ErrorMessage err;
            std::string content = res.getBody().getContent();
            switch (res.getType()) 
            {
                case 'e' :  err = utils::deserializeError(content);
                            if (err.number == NOTFND)
                                _error << "Question not found. "
                                       << err.extra;
                            else
                                _error << "Server error!" 
                                       << content;
                break;
                case 'o' : _output << content;
                break;
            }
            _output << std::endl;
        };

        return request;
    });

    _prepareRequest.emplace('q', [&](const std::string& line)
    {
        // TODO on non persistent, server does not need to
        //      be notified of client leaving
        
        Request request('q', "");

        // set handler
        _handleResponse = [&](const Response& res) {
            switch (res.getType()) {
                case 'e' : _error   << "Server error!" 
                                    << res.getBody().getContent();
                break;
                case 'o' : _output << "Server OK to leave!";
                           _running = false;
                break;
            }
            _output << std::endl;
        };

        return request;
    });

    _prepareRequest.emplace('c', [&](const std::string& line)
    {
        // build the message
        std::string buffer = line;

        // drop option part
        buffer.erase(0, 1);
        
        // get id
        size_t start = buffer.find_first_not_of(" ");
        size_t end   = buffer.find_first_of(" ", start + 1);
        std::string id = buffer.substr(start, end-1);
        buffer.erase(start, end);
        
        // get option
        start = buffer.find_first_not_of(" ");
        end   = buffer.find_first_of(" ", start + 1);
        std::string choice = buffer.substr(start, end);

        // compose
        std::string content = id + "\n" + choice + "\n";

        Request request('c', content);

        // set handler
        _handleResponse = [&](const Response& res) {
            ErrorMessage err;
            std::string content = res.getBody().getContent();
            switch (res.getType()) 
            {
                case 'e' :  err = utils::deserializeError(content);
                            if (err.number == NOTFND)
                                _error << "Question not found. "
                                       << err.extra << std::endl;
                            else if(err.number == CHNFND)
                                _error << "Choice not found. "
                                       << err.extra << std::endl;
                            else
                                _error << "Server error!" 
                                       << content;
                break;
                case 'o' : _output << content << std::endl;
                break;
            }
        };

        return request;
    });

    _prepareRequest.emplace('r', [&](const std::string& line)
    {
        // build the message
        Request request('r', "");

        // set handler
        _handleResponse = [&](const Response& res) {
            std::string content = res.getBody().getContent();
            switch (res.getType()) {
                case 'e' : _error   << "Server error!" 
                                    << res.getBody().getContent();
                break;
                case 'o' : 

                    // read the question
                    std::istringstream iss(content);
                    std::string id = "{id}";
                    std::getline(iss, id);
                    Question question(iss);
                    _output << id << std::endl 
                            << question.present() 
                            << "? " << std::flush;
                
                    std::string choice;
                    _input >> choice;
                    _input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    doRequest('c', "c " + id + " " + choice);

                break;
            }
            _output << std::endl;
        };

        return request;
    });

    _prepareRequest.emplace('k', [&](const std::string& line)
    {
        // build the message
        Request request('k', "");

        // set handler
        _handleResponse = [&](const Response& res) {
            switch (res.getType()) {
                case 'e' : _error   << "Server error!" 
                                    << res.getBody().getContent();
                break;
                case 'o' : _output << "Server going down!";
                           _running = false;
                break;
            }
            _output << std::endl;
        };

        return request;
    });
}