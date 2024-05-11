#include <iostream>
#include "Client.h"


using namespace std;


int main() {
    Client *client;
    try {
        client = new Client();
    } catch (bad_alloc &exception) {
        fprintf(stderr, "Client alloc failed.\n");
        exit(-1);
    }

    client->run();

    delete client;

    return 0;
}
