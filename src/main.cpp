#include "CServer.h"

int main(int argc, char **argv) {
    CServer server;

    //Check that the port number was passed as argument.
    if (argc < 2) {
        return 1;
    }

    //Start the server.
    if (server.bindSocket(std::stoi(std::string(argv[1]))) == 0) {
        server.run();
    } else {
        return 1;
    }

    return 0;
}
