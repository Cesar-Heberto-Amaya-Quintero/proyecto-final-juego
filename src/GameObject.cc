#include "GameObject.hh"

GameObject::GameObject(sf::Texture*& texture, float cropPosX, float cropPosY, float cropWidth,
 float cropHeight, float scaleX, float scaleY, b2Vec2* position,b2BodyType bodyType ,b2World*& world, sf::RenderWindow*& window)
{
    this->texture = texture;
    this->cropPosX = cropPosX;
    this->cropPosY = cropPosY;
    this->cropWidth = cropWidth;
    this->cropHeight = cropHeight;
    this->scaleX = scaleX;
    this->scaleY = scaleY;
    this->window = window;

    tileBaseWidth = cropPosX * scaleX;
    tileBaseHeight = cropPosY * scaleY;

    InitSprite(world, position, bodyType);

}

GameObject::~GameObject()
{
    boxCollider->~BoxCollider();
}

void GameObject::InitSprite(b2World*& world, b2Vec2*position, b2BodyType bodyType)
{
    sprite = new sf::Sprite(*texture, *(new sf::IntRect(cropPosX, cropPosY, cropWidth, cropHeight)));
    sprite->setScale(*(new sf::Vector2f(scaleX, scaleY)));

    Rigidbody* rb {new Rigidbody(world, bodyType, position, (cropWidth * scaleX) / 2, (cropHeight * scaleY) / 2, 1, 0, 0)};
    rb->SetUserData((void*) this);
    boxCollider = new BoxCollider(position->x, position->y, new sf::Color(0,255,0,255), cropWidth, cropHeight, rb, sprite);
    boxCollider->GetBoxShape()->setScale(scaleX, scaleY);
}

sf::Sprite* GameObject::GetSprite()
{
    return sprite;
}

void GameObject::SetPosition(float posX, float posY)
{
    sprite->setPosition(posX, posY);
}

BoxCollider* GameObject::GetCollider() const
{
    return boxCollider;
}

void GameObject::Update()
{
    boxCollider->UpdatePhysics();
    window->draw(*sprite);

}

const char* GameObject::GetTagName() const 
{
    return tagName;
}

void GameObject::SetTagName(const char* tagName)
{
    this->tagName = tagName;
}

bool GameObject::GetDebug() const
{
    return debug;
}

void GameObject::SetDebug(bool debug)
{
    this->debug = debug;
}