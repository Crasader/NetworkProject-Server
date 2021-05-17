#pragma once
#include "../../protobuf/src/game.pb.h"
#include "bullet.hpp"
#include "gameObject.hpp"
#include "playerObject.hpp"

#include <chrono>
#include <memory>
#include <vector>

class GameManager
{
private:
    /* data */
    std::vector<std::unique_ptr<GameObject>> gameObjList;
    GameState state;
    const static int w = 80;
    const static int h = 120;
    const static int threshold = h / 2;

public:
    GameManager(/* args */);
    GameState getState();
    void add(GameObject *gobj);
    int update(float deltaTime);
    void start();
    void end();
    void updatePlayerDir(uint8_t *buff, int player_id);
    int checkCollide();
    void addNewBullet(uint8_t *buff, int player_id);
    void updateBulletScreen();
    void dumpOutsideBullet(); //TODO: discard bullet that goes outside the screen
};