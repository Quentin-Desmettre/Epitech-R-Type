#include "rtype/RTypeServer.hpp"
#include "shared/ArgParser.hpp"

int main(int ac, char **av)
{
    srand(time(NULL));
    rtype::RTypeServer server(60, ac, av);

    server.run();
    return 0;
}
