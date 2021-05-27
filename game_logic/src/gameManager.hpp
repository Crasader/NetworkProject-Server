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
    const static int maxx = 61;
    const static int minx = 19;
    const static int maxh = 70;
    const static int minh = 30;
    const static int threshold = (maxh + minh) / 2;
    int bullet_count = 0; // use to assign i

public:
    GameManager(const GameManager &) = delete;
    GameManager &operator=(const GameManager &) = delete;
    GameManager(GameManager &&) = default;
    GameManager &operator=(GameManager &&) = default;
    GameManager();
    GameState getState();
    void add(GameObject *gobj);
    int update(float deltaTime);
    void start();
    void end();
    void updatePlayerDir(uint8_t *buff, int player_id, int size);
    int checkCollide();
    void addNewBullet(uint8_t *buff, int player_id);
    void updateBulletScreen();
    void dumpOutsideBullet(); //TODO: discard bullet that goes outside the screen
};
