#include "playerObject.hpp"

PlayerObject::PlayerObject(int maxHealth) : direction{0.0f, 0.0f}, currentHealth(maxHealth), speed(6){};

PlayerObject::~PlayerObject()
{
    printf("Now delete player object!\n");
}

void PlayerObject::update(float deltaTime, GameState &state)
{
    if (currentHealth <= 0)
    {
        state.hasFinish = true;
        return;
    }

    printf("The direction of the player: %f, %f\n", direction.x, direction.y);
    printf("The deltatime: %.4f\n", deltaTime);
    Vector dir_norm;
    dir_norm.x = direction.x / direction.magnitude();
    dir_norm.y = direction.y / direction.magnitude();
    if (direction.magnitude() > deltaTime * speed)
        position += dir_norm * speed * deltaTime;
    else
        position += direction * deltaTime;
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
    direction.x = dir.x;
    direction.y = dir.y;
}