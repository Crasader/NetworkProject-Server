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
    bool hasChangeScreen;
    bool shouldDiscard;
    int id;        // to distinguish during rendering process
    float radius;  //damage radius
    int screen_id; // 0 - player 1  screen, 1 - player2 screen
    BulletObject(int damage);
    ~BulletObject();
    Vector getDirection();
    int getDamage();
    void setDirection(Vector dir);
    void update(float deltaTime, GameState &state) override;
};
