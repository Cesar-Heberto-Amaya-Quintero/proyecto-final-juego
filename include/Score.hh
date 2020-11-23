#pragma once
#include <SFML/Graphics.hpp>

class Score
{
private:
    
    sf::Font* font{new sf::Font()};
    sf::Text* scoreText{new sf::Text()};
    sf::RenderWindow* window;
    int score{};
    std::string text;
public:
    Score(const char*, std::string, unsigned int, sf::Vector2f*, sf::Color*, sf::RenderWindow*&);
    ~Score();
    void update(float, float);
    void AddPoints(unsigned int);

    unsigned int GetPoints() const;
};


