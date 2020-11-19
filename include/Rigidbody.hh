#pragma once 
#include <box2d/box2d.h>

class Rigidbody
{
    private:
        b2BodyDef* bodyDef;
        b2Body* body;
        b2PolygonShape* polygonShape;
        b2FixtureDef* fixtureDef;
        b2Fixture* fixture;
        b2World* world;

    public:
        Rigidbody(b2World*&, b2BodyType, b2Vec2*, float, float, float, float, float);
        ~Rigidbody();
        b2Body* GetBody() const;
        void SetUserData(void*);
        void SetSensor(bool);
};


