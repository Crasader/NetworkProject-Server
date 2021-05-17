#pragma once

#include "gameObject.hpp"

class BulletObject : public GameObject
{
private:
    /* data */
    Vector direction;
    int damage;
    float speed;

public:
    float radius;  //damage radius
    int screen_id; // 0 - player 1  screen, 1 - player2 screen
    BulletObject(int damage, Vector v);
    ~BulletObject();
    Vector getDirection();
    void update(float deltaTime, GameState &state) override;
};
