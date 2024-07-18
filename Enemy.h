#include "BaseCharacter.h"
#include "Character.h"

class Enemy : public BaseCharacter
{
public:
    Enemy();
    Enemy(float health, float damage, float speed,
          Texture2D idle_texture, Texture2D run_texture, Vector2 pos);
    ~Enemy();

    virtual void tick(float deltaTime) override;
    virtual Vector2 getScreenPos() override;

    void setTarget(Character *character) { target = character; }

private:
    Character *target{nullptr};
    float radius{25.f};
};