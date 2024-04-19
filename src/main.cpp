#include "CServer.h"

int main(int argc, char **argv) {
    //Check that the port number and coin age flag were passed as argument.
    if (argc < 3) {
        return 1;
    }

    //Start the server.
    CServer server(std::stoi(std::string(argv[2])));
    if (server.bindSocket(std::stoi(std::string(argv[1]))) == 0) {
        server.run();
    } else {
        return 1;
    }

    return 0;
}
