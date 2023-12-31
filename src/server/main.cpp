#include "rtype/RTypeServer.hpp"
#include "shared/ArgParser.hpp"

int main(int ac, char **av)
{
    ArgParser parser(ac, av);
    std::cout << "port: " << parser.getPort() << std::endl;
    if (parser.getHelp()) {
        std::cout << "USAGE: ./rtype_client [-h | --help] [--port port]" << std::endl;
        return 0;
    }
    rtype::RTypeServer server(60);

    server.run();
    return 0;
}
