#include "GameObject.h"

//
//GameObject::GameObject()
//    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) { }

//GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color, glm::vec2 velocity)
//    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(
    float x, float y, float sizeX, float sizeY,
    Texture sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(x, y), Size(sizeX, sizeY),
    Velocity(velocity), Color(color),
    Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) {}
void GameObject::Draw(SpriteRenderer& renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}