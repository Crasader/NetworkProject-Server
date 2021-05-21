#pragma once
#include "gameState.hpp"
#include <math.h>
#include <string>

struct Vector
{
    float x;
    float y;

public:
    Vector() : x(0.0f), y(0.0f) {}
    Vector(float x, float y) : x(x), y(y) {}
    friend Vector operator+(const Vector &a, const Vector &b)
    {
        return Vector(a.x + b.x, a.y + b.y);
    }
    void operator+=(const Vector &other)
    {
        x = x + other.x;
        y = y + other.y;
    }
    friend Vector operator*(const Vector &a, float b)
    {
        return Vector(a.x * b, a.y * b);
    }

    float distance(Vector b)
    {
        return sqrt(pow(x - b.x, 2) + pow(y - b.y, 2));
    }
};
struct GameState;

class GameObject
{
protected:
    /* data */
    Vector position;

public:
    GameObject() : position{0.0f, 0.0f} {};
    virtual ~GameObject() = default;
    std::string tag; //determine whether or not a player
    int player_id;   //is this game object belong to player 1 or 2
    virtual void update(float deltaTime, GameState &state) = 0;
    virtual void onCollide(const GameObject *other){};
    Vector getPosition() { return position; };
    void setPosition(Vector pos)
    {
        position.x = pos.x;
        position.y = pos.y;
    };
};
