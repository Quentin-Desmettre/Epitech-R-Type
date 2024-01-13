//
// Created by Clément Vandeville on 08/01/2024.
//

#include "PongClient.hpp"

int main(int ac, char **av)
{
    PongClient client(60, 60, ac, av);

    client.run();
    return 0;
}
