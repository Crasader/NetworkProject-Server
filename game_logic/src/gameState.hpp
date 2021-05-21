#pragma once

#include <vector>

class GameObject;

struct GameState
{
    bool hasFinish;
    GameObject *player1;
    GameObject *player2;
    std::vector<GameObject *> bulletList1;
    std::vector<GameObject *> bulletList2;
};