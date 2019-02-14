#include "QuizClient.h"


QuizClient::QuizClient(IHost *host, bool persistent)
{
    _host = host;
    initHandlers();
}

void QuizClient::run(std::istream& is, 
                     std::ostream& os, 
                     std::ostream& errs)
{
    char option = '\0';
    os << "> " << std::flush;
    while((option = (char)is.get()) != 'q')
    {
        if(option == '\n')
        {
            os << "> " << std::flush;
            continue;
        }

        if(!_handlers.count(option))
            errs << "Invalid option, please try again." << std::endl;
        else
        {
            (_handlers.at(option))(option, is, os, errs, this);
        }

        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        os << "> " << std::flush;
    }
}

void QuizClient::initHandlers(void)
{
// p -> create a question ------------------------------------------------
    _handlers.emplace('p',
    [&](char option, 
        std::istream& is,
        std::ostream& os, 
        std::ostream& errs, 
        QuizClient* client)
    {
        // try to read the question and serialize it
        SolvedQuestion sq;
        
        try
        {
            sq = SolvedQuestion(is);
        } 
        catch (const std::exception& e) 
        {
            errs << "Invalid input: "
                 << std::string(e.what()) 
                 << std::endl;
            
            is.unget();

            return;
        }

        // build the message
        std::string msg = "";
        msg.push_back(option);
        msg.append(sq.serialize());
        std::istringstream iss(msg);
        
        Socket socket(IPv4, StreamSocket, TCP);
        // set the handler for outgoing message on client's socket
        socket.onOutgoingConnection = [&](
        ISocket& socket, const IHost& host, ISocket* context){
            socket.writeToSocket(iss);
            socket.readFromSocket(os);
        };
        
        // try to connect
        try
        {
            socket.connectTo(*(client->_host));
        }
        catch(const std::exception& e)
        {
            errs << "Failed to connect to server: " 
                 << e.what() << std::endl;
        }

        os << std::endl;
    });

// d -> delete a question ------------------------------------------------

    _handlers.emplace('d', 
    [&](char option, 
        std::istream& is, 
        std::ostream& os, 
        std::ostream& errs, 
        QuizClient* client)
    {
        // after the option, namely 'd', comes a space
        if(is.get() != ' ')
        {
            errs << "Invalid input: Missing space"
                 << " before the question number." << std::endl;
            return;
        }

        // then get rest of the line, should be a number
        std::string buff;
        std::getline(is, buff);
        is.unget(); // put back end-of-line
        if(buff.find_first_not_of("0123456789 ") != std::string::npos)
        {
            errs << "Invalid input: Question " 
                 << "id must be numeric." << std::endl;
            return;
        }
        
        // trim trailing and leading spaces
        buff.erase(buff.find_last_not_of(" \n\r\t")+1);
        buff = buff.substr(buff.find_first_not_of(" \n\r\t"));
        
        // build message and put in a input stream for tx
        std::string msg = "";
        msg.push_back(option);
        msg.push_back(' ');
        msg.append(buff);
        msg.push_back('\n');
        std::istringstream iss(msg); // "d ####\n"
        
        // get socket
        Socket socket(IPv4, StreamSocket, TCP);
        // set the handler for outgoing message on client's socket
        socket.onOutgoingConnection = [&](
        ISocket& socket, const IHost& host, ISocket* context){
            socket.writeToSocket(iss);
            socket.readFromSocket(os);
            os << std::endl;
        };
        
        // try to connect
        try
        {
            socket.connectTo(*(client->_host));
        }
        catch(const std::exception& e)
        {
            errs << "Failed to connect to server: " 
                 << e.what() << std::endl;
        }
    });

// g -> get a question ---------------------------------------------------

    _handlers.emplace('g', 
    [&](char option, 
        std::istream& is, 
        std::ostream& os, 
        std::ostream& errs, 
        QuizClient* client)
    {
        // after the option, namely 'g', comes a space
        if(is.get() != ' ')
        {
            errs << "Invalid input: Missing space"
                 << " before the question number." << std::endl;
            return;
        }

        // then get rest of the line, should be a number
        std::string buff;
        std::getline(is, buff);
        is.unget(); // put back end-of-line
        if(buff.find_first_not_of("0123456789 ") != std::string::npos)
        {
            errs << "Invalid input: Question " 
                 << "id must be numeric." << std::endl;
            return;
        }
        
        // trim trailing and leading spaces
        buff.erase(buff.find_last_not_of(" \n\r\t")+1);
        buff = buff.substr(buff.find_first_not_of(" \n\r\t"));
        
        // build message and put in a input stream for tx
        std::string msg = "";
        msg.push_back(option);
        msg.push_back(' ');
        msg.append(buff);
        msg.push_back('\n');
        std::istringstream iss(msg); // "g ####\n"
        
        // get socket
        Socket socket(IPv4, StreamSocket, TCP);
        // set the handler for outgoing message on client's socket
        socket.onOutgoingConnection = [&](
        ISocket& socket, const IHost& host, ISocket* context){
            // write demand
            socket.writeToSocket(iss);
            
            // read question until \n.\n
            std::string rbuff = "";
            std::string term = "\n.\n";
            size_t tL = term.length();

            // check for error msg too
            std::string errorMsg = 
            "Error: question " + buff + " not found.";
            
            do
            {
                std::ostringstream oss;
                socket.readFromSocket(oss);
                rbuff.append(oss.str());
            }while(rbuff.substr(rbuff.length() - tL - 1, tL) != term
                                                    && rbuff != errorMsg);
            
            os << rbuff << std::endl;
        };
        
        // try to connect
        try
        {
            socket.connectTo(*(client->_host));
        }
        catch(const std::exception& e)
        {
            errs << "Failed to connect to server: " 
                 << e.what() << std::endl;
        }
    });

// r -> get a random question --------------------------------------------

    _handlers.emplace('r',
    [&](char option, 
        std::istream& is,
        std::ostream& os, 
        std::ostream& errs, 
        QuizClient* client)
    {
        std::string msg = ""; 
        msg.push_back(option);
        std::istringstream iss(msg);
        
        Socket socket(IPv4, StreamSocket, TCP);
        // set the handler for outgoing message on client's socket
        socket.onOutgoingConnection = [&](
        ISocket& socket, const IHost& host, ISocket* context){
            socket.writeToSocket(iss);

            // read question until \n.\n
            std::string rbuff = "";
            std::string term = "\n.\n";
            size_t tL = term.length();
            
            // check for error msg too
            std::string errorMsg = "Error: There are no questions.";

            do
            {
                std::ostringstream oss;
                socket.readFromSocket(oss);
                rbuff.append(oss.str());
            }while(rbuff.substr(rbuff.length() - tL, tL) != term 
                                                && rbuff != errorMsg);

            if(rbuff == errorMsg)
            {
                errs << rbuff;
                return;
            }

            std::istringstream iss_q(rbuff);

            // get id
            std::string idLine = "";
            std::getline(iss_q, idLine);
            uint32_t id = (uint32_t)std::stoul(idLine);

            // create question
            Question q(iss_q);

            os << q.getQuestionTitle().getText() << std::endl;
            os << q.getChoices().getText();

            char input = '\0';
            os << "? " << std::flush;
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            input = is.get();
            is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            while(!q.getChoices().hasChoice(input))
            {   
                os << "The selected choice is not available." << std::endl;
                os << "? " << std::flush;
                input = is.get();
                is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            std::string choice = "";
            choice.push_back(input);
            std::istringstream iss_choice(choice);
            socket.writeToSocket(iss_choice);

            std::ostringstream reply;
            socket.readFromSocket(reply);
            os << reply.str();


        };
        
        // try to connect
        try
        {
            socket.connectTo(*(client->_host));
        }
        catch(const std::exception& e)
        {
            errs << "Failed to connect to server: " 
                 << e.what();
        }
        os << std::endl;
        is.unget(); // put back end-of-line
    });
}