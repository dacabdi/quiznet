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

void QuizClient::run(void)
{
    if(_persistent)
    {
        _output << "Using persistent connection to server." << std::endl;
        _socket = new Socket(IPv4, StreamSocket, TCP);
        _socket->connectTo(*_host);
        _output << "Connected to " << _host->getAddress() 
                << ":" << _host->getService() << std::endl;
    }
        
    char option = '\0';
    _output << "> " << std::flush;

    while((option = (char)_input.get()) != 'q')
    {
        _input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if(option == '\n')
        {
            _output << "> " << std::flush;
            continue;
        }

        if(!_prepareRequest.count(option))
            _error << "Invalid option, please try again." << std::endl;
        else
            doRequest(option);

        _output << "> " << std::flush;
    }
}

void QuizClient::doRequest(char option)
{
    // compose request
    std::string request;
    try {
        request = (_prepareRequest.at(option))();
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

struct Response QuizClient::sendAndReceive(const std::string& request)
{
    Response response;

    if(_persistent)
    {
        // write the request
        _socket->writeToSocket(request);
        // read the response
        response = parseResponse(*_socket);
    }
    else
    {
        Socket socket(IPv4, StreamSocket, TCP);
        socket.onOutgoingConnection = [&](
        ISocket& socket, const IHost& host, ISocket* context){
            // write the request
            socket.writeToSocket(request);
            // read the response
            response = parseResponse(socket);
            _output << std::endl;
        };
        // try to connect
        socket.connectTo(*_host);
    }

    return response;
}

struct Response QuizClient::parseResponse(ISocket& socket)
{
    std::string buffer = socket.readFromSocket();

    // read type
    char type = buffer.front();
    
    // read length
    ssize_t length = 0;
    ssize_t pos = buffer.find("\n");
    length = std::stoul(buffer.substr(2, pos));

    // get content
    std::string content = buffer.substr(pos+1, length);

    return { type, { length, content } };
}

void QuizClient::init(void)
{
    _prepareRequest.emplace('p', [&](void)
    {
        // NOTE: Since the data model SolvedQuestion object
        //       can be constructed by feeding it with an input stream
        //       I attempt to build the model on this side and take
        //       advantage of all the same validation rules that will
        //       be followed on the server side. Also, it takes care
        //       of delimiting the input and properly serializing it
        //       it back.

        // build the message
        SolvedQuestion sq(_input);
        std::string body = sq.serialize();
        std::string request = "p " + std::to_string(body.length()) + "\n"
                              + body + "\n";

        // set handler
        _handleResponse = [&](const struct Response& req) {
            switch (req.type)
            {
                case 'e' : 
                    _error << "Server error!" 
                           << req.body.content << std::endl;
                break;

                case 'o' :
                    _output << req.body.content << std::flush;
                break;
            }
        };

        return request;
    });
}