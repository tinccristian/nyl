// #include "GameObject.h"


// GameObject::GameObject(
//     float x, float y, float sizeX, float sizeY,
//     Texture sprite, glm::vec3 color, glm::vec2 velocity,
//     Physics* physics, Collider* collider)
//     : Position(x, y), Size(sizeX, sizeY),
//     Velocity(velocity), Color(color),
//     Rotation(0.0f),Direction(1.0f), Sprite(sprite), IsSolid(false), Destroyed(false),
//     physicsComponent(physics), colliderComponent(collider) {}
// void GameObject::Draw(SpriteRenderer& renderer)
// {
//     //this->Direction = this->Velocity.x >= 0 ? 1.0f : -1.0f;
//     renderer.DrawObject(this->Sprite, this->Position, this->Size, this->Rotation, this->Color, this->Direction);
// }