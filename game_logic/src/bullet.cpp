#include "bullet.hpp"

BulletObject::BulletObject(int damage) : damage(damage), direction{0.0f, 0.0f}, speed(8), radius(0.5), hasChangeScreen(false), shouldDiscard(false) {}

BulletObject::~BulletObject()
{
}

Vector BulletObject::getDirection()
{
    return direction;
}

int BulletObject::getDamage()
{
    return damage;
}

void BulletObject::setDirection(Vector dir)
{
    direction.x = dir.x;
    direction.y = dir.y;
}

void BulletObject::update(float deltaTime, GameState &state)
{
    position += direction * deltaTime * speed;
    printf("Bullet update: %f %f\n", direction.x, direction.y);
    printf("Bullet update: %f %f\n", position.x, position.y);
}