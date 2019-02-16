#include "Exception.h"
#include "QuizClient.h"
#include "Host.h"

#include <iostream>

int main(int argc, char *argv[])
{   
    std::string usage(
    "Usage: qclient TARGETHOST PORT [persistency]\n"
    "\nParameters:\n\n"
    "\tTARGETHOST  : IP address or hostname of server.\n"
    "\tPORT        : TARGETHOST's port.\n"
    "\tpersistency : Connection mode (\"persistent\" or \"nonpersistent\").\n"
    "\t              Client is persistent by default.\n\n");

    if(argc < 3) 
    {
        std::cerr << "ERROR: Please specify server's host and port.\n" 
                  << std::endl;
        std::cout << usage << std::flush;
        exit(EXIT_FAILURE);
    }

    std::string persistency = "persistent";
    if(argc == 4)
    {
        persistency = argv[3];
        
        if (!(persistency == "nonpersistent" 
           || persistency == "persistent"))
        {
            std::cerr << "Cannot recognize persistency mode: " 
                      << persistency << std::endl;
            std::cout << usage << std::flush;
            exit(EXIT_FAILURE);
        }
    }

    std::string welcome(
    "\n\n/==============================================\\\n"
        "|                QuizNet Client                |\n"
       "\\==============================================/\n");
         
    std::cout << welcome << std::flush;

    std::cout << "\n\n------------ Initialization settings" 
              << "-----------\n" << std::endl;
    std::cout << "\tTarget server : " 
              << argv[1] << ":" << argv[2] << std::endl;
    std::cout << "\tPersistency   : " 
              << persistency << "\n" << std::endl;

    Host host(argv[1], argv[2]);
    QuizClient client(&host, persistency =="persistent");
    client.run();

    std::cout << "\nBh-Bh-Bye! :( \n" << std::endl;

    exit(EXIT_SUCCESS); 
}