#include "bullet.hpp"

BulletObject::BulletObject(int damage, Vector v) : damage(damage), direction(v), speed(5), radius(5) {}

BulletObject::~BulletObject()
{
}

Vector BulletObject::getDirection()
{
    return direction;
}

void BulletObject::update(float deltaTime, GameState &state)
{
    position += direction * deltaTime * speed;
}