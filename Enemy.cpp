#include "Enemy.h"
#include "raymath.h"

Enemy::Enemy()
{
}

Enemy::Enemy(float health, float damage, float speed,
             Texture2D idle_texture, Texture2D run_texture, Vector2 pos)
    : BaseCharacter(health, damage, speed, idle_texture, run_texture)
{
    worldPos = pos;
    width = texture.width / maxFrame;
    height = texture.height;
}

Enemy::~Enemy()
{
}

Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}

void Enemy::tick(float deltaTime)
{
    if (!getAlive())
        return;

    // get toTarget
    velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());

    if (Vector2Length(velocity) < radius)
        velocity = {};

    BaseCharacter::tick(deltaTime);

    if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec()))
    {
        target->takeDamage(damagePerSec * deltaTime);
    }
}