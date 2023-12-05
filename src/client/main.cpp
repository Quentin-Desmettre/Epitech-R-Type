#include "rtype/RTypeClient.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    rtype::RTypeClient client(60, 60);

    client.run();

    return 0;
}
