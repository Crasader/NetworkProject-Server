#include "client.hpp"

float ran_float(int limit)
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX / limit);
}

int main(int argc, char const *argv[])
{
    Client cli = Client(9999);
    while (true)
    {
        uint8_t buffer[4096 + 2];
        cli.receiveData(buffer, 4096 + 1);

        cli.sendDirection(ran_float(1), ran_float(1));

        if (ran_float(1) > 0.5)
            cli.sendShoot();
    }

    return 0;
}