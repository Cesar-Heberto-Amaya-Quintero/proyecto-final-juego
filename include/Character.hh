#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.hh"
#include "BoxCollider.hh"
#include "Animation.hh"

class Character : public GameObject
{
    private:

        Animation** animations;

        void InitSprite(b2World*&, b2Vec2*);
        b2Vec2* position;
    public:
        Character(sf::Texture*&, float, float, float, float, float, float, b2Vec2*,b2BodyType,b2World*&, sf::RenderWindow*&); 
        ~Character();
        
        void FlipSpriteX(float x);
        b2Vec2 GetCharacterPosition() const;
        void Move(b2Vec2*);
        void SetAnimations(Animation**);
        Animation* GetAnimation(int) const;
        
};


