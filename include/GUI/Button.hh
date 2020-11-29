#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Button
{
    private:
        int windowLocation{};
        sf::RectangleShape* rectangleShape;
        float posX, posY, width, height, borderSize;
        sf::Color* fillColor;
        sf::Color* borderColor;
        sf::RenderWindow* window;
    public:
        void Init();
        void Update();
        void UpdateStart();
        void OnClick();
        void OnClickStart();
        int findWindow() const;
        Button(float, float, float, float, float, sf::Color*, sf::Color*, sf::RenderWindow*&);
        ~Button();
};