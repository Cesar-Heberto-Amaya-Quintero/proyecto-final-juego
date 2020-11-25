#include "ContactListener.hh"
#include "GameObject.hh"
#include "Tile.hh"
#include <iostream>
#include <cstring>
#include <algorithm>
#include <type_traits>

ContactListener::ContactListener(Score*& score, std::vector<GameObject*>*& items)
{
    this->score = score;
    this->items = items;
}

ContactListener::~ContactListener()
{
}

int ContactListener::GetSceneIndex() const
{
    return sceneIndex;
}

void ContactListener::BeginContact(b2Contact* contact)
{
    GameObject* bodyDataA = (GameObject*) contact->GetFixtureA()->GetBody()->GetUserData();
    GameObject* bodyDataB = (GameObject*) contact->GetFixtureB()->GetBody()->GetUserData();
    if (bodyDataA && bodyDataB)
    {
        if (std::strcmp(bodyDataA->GetTagName(), "player") == 0 && std::strcmp(bodyDataB->GetTagName(), "treasure") ==0)
        {   //SI son iguales por eso el 0
            
            score->AddPoints(10);
            //items->erase(std::remove(items->begin(), items->end(), bodyDataB), items->end());
            //bodyDataB->~GameObject();
            
        }
        if(std::strcmp(bodyDataA->GetTagName(), "player") == 0 && std::strcmp(bodyDataB->GetTagName(), "meat") == 0)
        {
            score->AddPoints(10);
            //std::cout << "stairs" << std::endl;
            items->erase(std::remove(items->begin(), items->end(), bodyDataB), items->end());
            bodyDataB->~GameObject();
        }
        if(std::strcmp(bodyDataA->GetTagName(), "player") == 0 && std::strcmp(bodyDataB->GetTagName(), "enemy") == 0)
        {
            gameOver=true;
            items->erase(std::remove(items->begin(), items->end(), bodyDataB), items->end());
            bodyDataB->~GameObject();
        }

    }
}

void ContactListener::EndContact(b2Contact* contact)
{

}

bool ContactListener::IsGameOver() const 
{
    return gameOver;
}

void ContactListener::ResetGameOver(bool resetGameOver)
{
    gameOver= resetGameOver;
}
