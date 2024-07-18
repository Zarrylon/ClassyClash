#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H

#include "raylib.h"

class BaseCharacter
{
public:
    BaseCharacter();
    BaseCharacter(float health, float damage, float speed,
                  Texture2D idle_texture, Texture2D run_texture);
    ~BaseCharacter();

    virtual void tick(float deltaTime) = 0;
    virtual Vector2 getScreenPos() = 0;

    Rectangle getCollisionRec();
    void takeDamage(float damage);
    void showHealth();

    float getTotalHealth() const { return totalHealth; }
    float getHealth() const { return currentHealth; }
    float getDamage() const { return damagePerSec; }
    float getSpeed() const { return movementSpeed; }

    Vector2 getWorldPos() const { return worldPos; }
    bool getAlive() const { return alive; }

protected:
    // stats
    float totalHealth{};
    float currentHealth{};
    float damagePerSec{};
    float movementSpeed{};

    // texture
    Texture2D texture{};
    Texture2D idle{};
    Texture2D run{};

    // position
    Vector2 worldPos{};
    Vector2 worldPosLastFrame{};
    Vector2 velocity{};

    // animation frame
    int maxFrame{6};
    int frame{};

    // animation time
    float updateTime{1.f / 12.f};
    float runningTime{};

    // direction: 1 = right | -1 = left
    float rightLeft{1.f};

    // dimensions
    float width{};
    float height{};
    float scale{4.0f};

private:
    bool alive{true};
};

#endif