#include <iostream>
#include "Server.h"

int main(void)
{
    Server server("8080");
    server.run();

    return 0;
}