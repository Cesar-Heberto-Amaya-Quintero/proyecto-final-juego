#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameObject.hh"
#include "Score.hh"
#include "SFX.hh"


class ContactListener : public b2ContactListener 
{
    private:

        Score* score;
        std::vector<GameObject*>* items;
        int sceneIndex{};
        bool gameOver{false};
        SFX* sfx;

    public: 
        ContactListener(Score*&, std::vector<GameObject*>*&);
        ~ContactListener();

        void BeginContact(b2Contact*);
        void EndContact(b2Contact*);
        int GetSceneIndex() const;
        bool IsGameOver() const;
        float GetDelay() const;
        void ResetGameOver(bool);    
};