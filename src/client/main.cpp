#include "rtype/RTypeClient.hpp"
#include "shared/ArgParser.hpp"

int main(int ac, char **av)
{
    ArgParser parser(ac, av);
    std::cout << "ip: " << parser.getIp() << std::endl;
    std::cout << "port: " << parser.getPort() << std::endl;
    if (parser.getHelp()) {
        std::cout << "USAGE: ./rtype_client [-h | --help] [--ip ip] [--port port]" << std::endl;
        return 0;
    }

    rtype::RTypeClient client(60, 60);

    client.run();

    return 0;
}
