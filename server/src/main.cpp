#include "server.hpp"

int main(int argc, char const *argv[])
{
    Server server = Server(9999);
    server.mainLogic();
    return 0;
}
