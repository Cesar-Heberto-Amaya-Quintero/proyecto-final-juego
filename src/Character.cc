#include "Character.hh"

Character::Character(sf::Texture*& texture, float cropPosX, float cropPosY, float cropWidth,
float cropHeight, float scaleX, float scaleY, b2Vec2* position,b2BodyType bodyType,b2World*& world, sf::RenderWindow*& window) :
GameObject(texture, cropPosX,cropPosY, cropWidth, cropHeight, scaleX, scaleY, position, bodyType,world, window)
{
    this->position= position;
}

void Character::Move(b2Vec2* direction)
{
    boxCollider->Move(direction);
}

void Character::FlipSpriteX(float x)
{
    if (x>0)
    {
        sprite->setScale(scaleX, scaleY);
        sprite->setOrigin(0.f, 0.f);
    }
    if (x<0)
    {
        sprite->setScale(-scaleX, scaleY);
        sprite->setOrigin(sprite->getGlobalBounds().width/scaleX, 0.f);
    }
}

b2Vec2 Character::GetCharacterPosition() const
{
    return boxCollider->GetBodyPosition();
}

void Character::SetAnimations(Animation** animations)
{
    this->animations = animations;
}

Animation* Character::GetAnimation(int index) const
{
    return *(animations + index);
}

Character::~Character(){}
