//
// Created by Clément Vandeville on 08/01/2024.
//

#include "server/PongServer.hpp"

int main(int ac, char **av)
{
    pong::PongServer server(60, ac, av);

    server.run();
    return 0;
}
