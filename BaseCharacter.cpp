#include "BaseCharacter.h"
#include "raymath.h"
#include <string>

BaseCharacter::BaseCharacter()
{
}

BaseCharacter::BaseCharacter(float health, float damage, float speed,
                             Texture2D idle_texture, Texture2D run_texture)
    : totalHealth(health),
      currentHealth(health),
      damagePerSec(damage),
      movementSpeed(speed),
      texture(idle_texture),
      idle(idle_texture),
      run(run_texture)
{
}

BaseCharacter::~BaseCharacter()
{
    if (texture.id == 0)
        UnloadTexture(texture);
    if (idle.id == 0)
        UnloadTexture(idle);
    if (run.id == 0)
        UnloadTexture(run);
}

void BaseCharacter::tick(float deltaTime)
{
    worldPosLastFrame = worldPos;

    showHealth();

    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        runningTime = 0.f;
        frame++;
        if (frame > maxFrame)
            frame = 0;
    }

    if (Vector2Length(velocity) != 0.0)
    {
        // set worldPos = worldPos + direction
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), movementSpeed));
        velocity.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;
        texture = run;
    }
    else
        texture = idle;

    velocity = {};

    // draw character
    Rectangle source{
        frame * width,
        0.f,
        rightLeft * width,
        height};

    Rectangle dest{
        getScreenPos().x,
        getScreenPos().y,
        scale * width,
        scale * height};

    Vector2 origin{};
    DrawTexturePro(texture, source, dest, origin, 0.f, WHITE);
}

Rectangle BaseCharacter::getCollisionRec()
{
    return Rectangle{
        getScreenPos().x,
        getScreenPos().y,
        width * scale,
        height * scale};
}

void BaseCharacter::takeDamage(float damage)
{
    currentHealth -= damage;
    if (currentHealth <= 0.f)
        alive = false;
}

void BaseCharacter::showHealth()
{
    float margin{20.f};
    Vector2 healthPos{getScreenPos().x, getScreenPos().y - margin};

    DrawRectangle(healthPos.x, healthPos.y, texture.width, texture.height, BLACK);
    DrawRectangle(healthPos.x, healthPos.y, texture.width * currentHealth / totalHealth, texture.height, RED);

    std::string health = "";
    health.append(std::to_string(currentHealth), 0, 4);
    DrawText(health.c_str(), healthPos.x + texture.width / 3.5f, healthPos.y, 20, WHITE);
}