#include "server/src/server.hpp"

int main(int argc, char const *argv[])
{
    Server sv = Server(9999);
    sv.mainLogic();

    return 0;
}
