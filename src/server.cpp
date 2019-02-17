#include "Host.h"
#include "QuizBook.h"
#include "QuizServer.h"

#include <iostream>

int main(int argc, char *argv[])
{   
    std::string usage(
    "Usage: qserver [PORT] [repository] [verbose]\n"
    "\nParameters:\n\n"
    "\tPORT       : Port to run on. Automatically assigned (0) by default.\n"
    "\trepository : Path to file for quiz repository.\n"
    "\tverbose    : Display requests and responses.\n\n");

    std::string port = "0";
    if(argc == 2)
        port = std::string(argv[1]);
    
    std::string quizbookFilename = "quizserver-repo.data";

    // if file provided
    if(argc == 3)
        quizbookFilename = argv[2];

    std::string welcome(
    "\n\n/============================================================\\\n"
        "|                       QuizNet Server                       |\n"
       "\\============================================================/\n");

    std::string verbose = "Non verbose";
    if(argc == 4)
    {
        if(std::string(argv[3]) == "verbose")
            verbose = argv[3];
        else
        {
            std::cerr << "ERROR: Verbosity parameter not recognized: " 
                      << argv[3] << std::endl;
            std::cout << usage << std::flush;
            exit(EXIT_FAILURE);
        }
    }

    std::cout << welcome << std::flush;

    std::cout << "\n------------------ Initialization settings";
    std::cout <<  " ------------------\n" << std::endl;
    std::cout << "\tPort         : " << (port == "0" ? "Any" : port) 
                                     << std::endl;
    std::cout << "\tRepository   : " << quizbookFilename << std::endl;
    std::cout << "\tVerbose      : " << verbose << std::endl << std::endl;

    QuizBook quizbook;
    Host host(port);
    Socket socket(IPv4, StreamSocket, TCP);
    
    QuizServer server(&quizbook, &host, &socket, quizbookFilename);
    server.setLoggerVerbose(verbose == "verbose");
    server.setLogger([&](const std::string& str){
        std::cout << str << std::flush;
    });

    server.run();

    std::cout << "\nServer out!\nBh-Bh-Bye! :( \n" << std::endl;

    exit(EXIT_SUCCESS);
}
