#include "rtype/RTypeServer.hpp"
#include "shared/ArgParser.hpp"

int main(int ac, char **av)
{
    rtype::RTypeServer server(60, ac, av);

    server.run();
    return 0;
}
