#include "rtype/RTypeClient.hpp"

int main(int ac, char **av)
{
    rtype::RTypeClient client(60, 60, ac, av);

    client.run();

    return 0;
}
