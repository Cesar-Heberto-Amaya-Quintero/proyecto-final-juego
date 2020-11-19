#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Vec2.hh"

struct Inputs 
{
    float x;
    float y;

    bool isJumping {false};

    Vec2* keyboardAxis;
    Vec2* joystickAxis;

    Inputs();

    Vec2* GetKeyboardAxis(); 
    Vec2* GetJoystickAxis(); 

    bool GetIsJumping();


};
