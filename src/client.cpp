#include "Client.h"

int main(void)
{
    Client client;
    client.setServer("localhost", "8080");
    client.talk("hello there!");

    return 0;
}