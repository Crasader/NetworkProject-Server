#include "gameManager.hpp"

GameManager::GameManager(/* args */)
{
    // PlayerObject *player1 = new PlayerObject(100);
    gameObjList.push_back(std::make_unique<PlayerObject>(100));
    gameObjList.push_back(std::make_unique<PlayerObject>(100));

    gameObjList[0]->tag = "player";
    gameObjList[0]->player_id = 0;
    gameObjList[0]->setPosition(Vector(40, threshold - 50));

    gameObjList[1]->tag = "player";
    gameObjList[0]->player_id = 1;
    gameObjList[1]->setPosition(Vector(40, threshold + 50));
}

GameState GameManager::getState()
{
    return state;
}
// 0 - player 1 win, 1 - player 2 win, 2 - still running
int GameManager::update(float deltaTime)
{
    printf("The length of gameObjList: %lu\n", gameObjList.size());

    for (auto &gameobj : gameObjList)
    {
        printf("Tag of object: %s\n", gameobj->tag.c_str());
        gameobj->update(deltaTime, state);

        if (state.hasFinish)
        {
            return false;
        }
    }
    updateBulletScreen();
    int p_status = checkCollide();
    //TODO: calculate stop citeria
    switch (p_status)
    {
    case 0:
    {
        int health = ((PlayerObject *)gameObjList[0].get())->getHealth();
        if (health <= 0) // player 1 die -> p2 win
            return 1;
        break;
    }
    case 1:
    {
        int health = ((PlayerObject *)gameObjList[1].get())->getHealth();
        if (health <= 0)
            return 0;
        break;
    }
    default: // -1
        break;
    }
    return 2;
}

void GameManager::start()
{
    // isRunning = true;
    // lastUpdate = std::chrono::high_resolution_clock::now();
    // while (true && isRunning)
    // {
    //     update();
    // }

    // end();
}

void GameManager::updatePlayerDir(uint8_t *buff, int player_id, int size)
{
    printf("Update directions for %d:\n", player_id);
    //get the player direction
    MovingDirection dir;
    dir.ParseFromArray(buff, size);

    Vector direction = Vector(dir.vx(), dir.vy());
    printf("Vx: %f, Vy: %f\n", direction.x, direction.y);

    // for (auto &gameobj : gameObjList)
    // {
    //     if (gameobj->tag.compare("player") != 0 || gameobj->player_id != player_id)
    //         continue;
    //     else
    //         ((PlayerObject *)gameobj.get())->setDirection(direction);
    // }
    ((PlayerObject *)gameObjList[player_id].get())->setDirection(direction);
}

void GameManager::updateBulletScreen()
{
    // std::vector<BulletObject *> screen1Bulls;
    // std::vector<BulletObject *> screen2Bulls;

    state.bulletList1.clear();
    state.bulletList2.clear();
    for (auto &gameobj : gameObjList)
    {
        if (gameobj->tag == "player")
            continue;
        if (((BulletObject *)gameobj.get())->getPosition().y < threshold) //screen 1
        {
            ((BulletObject *)gameobj.get())->screen_id = 0;
            state.bulletList1.push_back((BulletObject *)gameobj.get());
        }
        else
        {
            ((BulletObject *)gameobj.get())->screen_id = 1;
            state.bulletList2.push_back((BulletObject *)gameobj.get());
        }
    }

    //reassign list of bullet at screen 1 and screen 2
    // state.bulletList1 = screen1Bulls;
    // state.bulletList2 = screen2Bulls;
}

void GameManager::addNewBullet(uint8_t *buff, int player_id)
{
    //add new bullet at the player' corresponding position

    printf("Spawning bullet...\n");

    //set new bullet at the player postition, and having the same direction as the player
    // BulletObject *bo = new BulletObject(10, ((PlayerObject *)gameobj.get())->getDirection());
    // bo->setPosition(((PlayerObject *)gameobj.get())->getPosition());
    // bo->player_id = gameobj->player_id;
    // bo->screen_id = bo->player_id;

    //add to the manage list of game manager
    gameObjList.push_back(std::make_unique<BulletObject>(10, ((PlayerObject *)gameObjList[player_id].get())->getDirection()));
    BulletObject *bo = (BulletObject *)gameObjList[gameObjList.size() - 1].get();
    bo->setPosition(((PlayerObject *)gameObjList[player_id].get())->getPosition());
    bo->player_id = player_id;
    bo->screen_id = player_id;
    bo->tag = "bullet";
}

// 0 - player 1 collide, 1 - player 2 collide, -1 - no collision
int GameManager::checkCollide()
{
    for (int player_id = 0; player_id < 2; player_id++)
    {
        PlayerObject *player = (PlayerObject *)gameObjList[player_id].get();
        //check collide bw player vs border
        Vector player_pos = player->getPosition();
        if (player_pos.x < 0)
        {
            player_pos.x = 0;
        }
        if (player_pos.x > w)
        {
            player_pos.x = w;
        }
        if (player_pos.y < 0)
        {
            player_pos.y = 0;
        }
        if (player_pos.y > threshold)
        {
            player_pos.y = threshold;
        }
        player->setPosition(player_pos);

        for (int i = 2; i < gameObjList.size(); i++)
        {
            BulletObject *bullet = (BulletObject *)gameObjList[i].get();
            //check collide bw bullet and player
            if (bullet->tag == "bullet" && bullet->player_id != player_id)
            {
                // check if the bullet of other player hit this player
                float dis = player->getPosition().distance((bullet)->getPosition());
                if (dis < bullet->radius) //get collide
                {
                    printf("Collide between player and bullet\n");
                    player->onCollide(bullet);

                    //dispose collided bullet
                    GameObject *tmp = bullet;
                    gameObjList.erase(gameObjList.begin() + i);
                    return player->player_id;
                }
            }

            //check collide bw bullet vs border
            Vector bullet_pos = bullet->getPosition();
            if (bullet_pos.x < 0 || bullet_pos.x > w || bullet_pos.y < 0 || bullet_pos.y > h)
            {
                GameObject *tmp = bullet;
                gameObjList.erase(gameObjList.begin() + i);
            }
        }
    }

    //update the state of the two player to gameState
    state.player1 = (PlayerObject *)gameObjList[0].get();
    state.player2 = (PlayerObject *)gameObjList[1].get();
    return -1;
}