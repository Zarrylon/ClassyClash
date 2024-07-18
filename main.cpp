#include <iostream>
#include <string>
#include <random>
#include <vector>

#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"

float getRandomValue(float min, float max)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> distibutive(min, max);

    return distibutive(generator);
}

int getRandomValue(int min, int max)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distibutive(min, max);

    return distibutive(generator);
}

Enemy spawnEnemy()
{
    int health = getRandomValue(1, 200);
    float damage = getRandomValue(1.f, 50.f);
    float speed = getRandomValue(1.f, 5.f);
    Vector2 position{getRandomValue(200.f, 2000.f), getRandomValue(200.f, 1000.f)};
    int enemy = getRandomValue(1, 2);

    Texture2D idleTexture{};
    Texture2D runTexture{};

    // only 2 type of enemies, so if is ok here
    if (enemy == 1)
    {
        idleTexture = LoadTexture("characters/goblin_idle_spritesheet.png");
        runTexture = LoadTexture("characters/goblin_run_spritesheet.png");
    }
    else
    {
        idleTexture = LoadTexture("characters/slime_idle_spritesheet.png");
        runTexture = LoadTexture("characters/slime_run_spritesheet.png");
    }

    return Enemy{
        static_cast<float>(health),
        damage,
        speed,
        idleTexture,
        runTexture,
        position};
}

int main()
{
    const int windowDimensions[2]{384, 384};
    InitWindow(windowDimensions[0], windowDimensions[1], "Zarrylon - Classy Clash");

    Texture2D map = LoadTexture("nature_tileset/WorldMap.png");
    Vector2 mapPos{};

    const float mapScale{3.0f};

    Character knight{
        150.f,
        25.f,
        4.f,
        LoadTexture("characters/knight_idle_spritesheet.png"),
        LoadTexture("characters/knight_run_spritesheet.png"),
        Vector2{static_cast<float>(windowDimensions[0]), static_cast<float>(windowDimensions[1])}};

    Texture2D propsVariant[]{
        LoadTexture("nature_tileset/Rock.png"),
        LoadTexture("nature_tileset/Log.png"),
        LoadTexture("nature_tileset/Bush.png")};

    Prop props[]{
        Prop{propsVariant[0], Vector2{600.f, 300.f}},
        Prop{propsVariant[1], Vector2{400.f, 500.f}}};

    const int enemyCount{5};
    Enemy enemyArray[enemyCount];
    Enemy *enemies[enemyCount];

    for (int i = 0; i < enemyCount; i++)
    {
        enemyArray[i] = spawnEnemy();
        enemies[i] = &enemyArray[i];
    }

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // start game logic
        float deltaTime = GetFrameTime();
        mapPos = Vector2Scale(knight.getWorldPos(), -1.f);

        // draw map
        DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);

        // draw props
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        if (!knight.getAlive())
        {
            DrawText("Game over", 90.f, 45.f, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            knight.tick(deltaTime);

            // check map bounds
            if (knight.getWorldPos().x < 0.f ||
                knight.getWorldPos().y < 0.f ||
                knight.getWorldPos().x + windowDimensions[0] > map.width * mapScale ||
                knight.getWorldPos().y + windowDimensions[1] > map.height * mapScale)
            {
                knight.undoMovement();
            }

            for (auto prop : props)
            {
                if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
                {
                    knight.undoMovement();
                }
            }
        }

        int currentEnemies = enemyCount;
        for (auto enemy : enemies)
        {
            enemy->tick(deltaTime);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
            {
                enemy->takeDamage(knight.getDamage());
            }

            if (!enemy->getAlive())
                currentEnemies--;
        }

        if (currentEnemies == 0)
        {
            DrawText("VICTORY!!!", 90.f, 45.f, 40, WHITE);
            EndDrawing();
            continue;
        }

        // end game logic
        EndDrawing();
    }

    UnloadTexture(map);
    CloseWindow();
}