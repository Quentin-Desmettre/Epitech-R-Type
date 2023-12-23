#include "rtype/RTypeServer.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    rtype::RTypeServer server(60);

    server.run();
    return 0;
}
