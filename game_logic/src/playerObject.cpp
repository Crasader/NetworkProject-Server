#include "playerObject.hpp"

PlayerObject::PlayerObject(int maxHealth) : currentHealth(maxHealth), speed(7){};

PlayerObject::~PlayerObject()
{
}

void PlayerObject::update(float deltaTime, GameState &state)
{
    if (currentHealth <= 0)
    {
        state.hasFinish = true;
        return;
    }
    position += direction * deltaTime * speed;
}

void PlayerObject::onCollide(const GameObject *other)
{
    currentHealth -= 10;
};
Vector PlayerObject::getDirection()
{
    return direction;
}

int PlayerObject::getHealth()
{
    return currentHealth;
}

void PlayerObject::setDirection(Vector dir)
{
    direction = dir;
}