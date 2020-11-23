#pragma once
#include<SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <BoxCollider.hh>

class GameObject
{
    protected:
        sf::Texture* texture;
        sf::Sprite* sprite;
        float cropPosX, cropPosY, cropWidth, cropHeight;
        float scaleX,scaleY;
        BoxCollider* boxCollider;
        float tileBaseWidth;
        float tileBaseHeight;
        sf::RenderWindow* window;

        const char* tagName{"default"};

        bool debug{};

        void InitSprite(b2World*&, b2Vec2*, b2BodyType);

    public:
        GameObject(sf::Texture*&, float, float, float, float, float, float, b2Vec2*,b2BodyType,b2World*&, sf::RenderWindow*&);
        ~GameObject();

        sf::Sprite* GetSprite();
        BoxCollider* GetCollider() const;
        void Update();
        void SetPosition(float,float);
        const char* GetTagName() const;
        void SetTagName (const char*);

        void SetDebug(bool);
        bool GetDebug() const;
};


