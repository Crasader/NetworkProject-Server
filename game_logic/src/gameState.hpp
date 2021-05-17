#pragma once
#include "bullet.hpp"
#include "playerObject.hpp"

#include <vector>

class GameState
{
    bool hasFinish;
    PlayerObject *player1;
    PlayerObject *player2;
    std::vector<BulletObject *> bulletList1;
    std::vector<BulletObject *> bulletList2;
};