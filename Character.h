#ifndef CHARACTER_H
#define CHARACTER_H

#include "BaseCharacter.h"

class Character : public BaseCharacter
{
public:
    Character();
    Character(float health, float damage, float speed,
              Texture2D idle_texture, Texture2D run_texture, Vector2 pos);
    ~Character();

    Rectangle getWeaponCollisionRec() const { return weaponCollisionRec; }

    virtual void tick(float deltaTime) override;
    virtual Vector2 getScreenPos() override;

    void undoMovement();

private:
    int windowWidth{};
    int windowHeight{};

    Texture2D weapon{};
    Rectangle weaponCollisionRec{};
};

#endif