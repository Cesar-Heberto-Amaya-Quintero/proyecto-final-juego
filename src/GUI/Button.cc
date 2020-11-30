#include "GUI/Button.hh"

Button::Button(float posX, float posY, float width, float height, float borderSize, sf::Color* fillColor, sf::Color* borderColor, sf::RenderWindow*& window)
{
    this->posX = posX;
    this->posY = posY;
    this->width = width;
    this->height = height;
    this->borderSize = borderSize;
    this->fillColor = fillColor;
    this->borderColor = borderColor;
    this->window = window;
     Init();
}

void Button::Init()
{
    rectangleShape = new sf::RectangleShape();
    rectangleShape->setPosition(*(new sf::Vector2f(posX, posY)));
    rectangleShape->setSize(*(new sf::Vector2f(width, height)));
    rectangleShape->setFillColor(*fillColor);
    rectangleShape->setOutlineColor(*borderColor);
    rectangleShape->setOutlineThickness(borderSize);
}

void Button::Update()
{
    OnClick();
    window->draw(*rectangleShape);
}  

void Button::UpdateStart()
{
    OnClickStart();
    window->draw(*rectangleShape);
} 

void Button::OnClick()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);//Captura si estamos en el area de la ventana de nuestro juego
    sf::Vector2f mouseTranslate =  window->mapPixelToCoords(mousePos); //Este captura cuanto se ha movido el mouse dentro de la vena
    if(rectangleShape->getGlobalBounds().contains(mouseTranslate) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) //Si esa traslación fue sobre la forma de nuestro rectangulo
    {
        
        windowLocation = 3;
    }
}

void Button::OnClickStart()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);//Captura si estamos en el area de la ventana de nuestro juego
    sf::Vector2f mouseTranslate =  window->mapPixelToCoords(mousePos); //Este captura cuanto se ha movido el mouse dentro de la vena
    if(rectangleShape->getGlobalBounds().contains(mouseTranslate) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) //Si esa traslación fue sobre la forma de nuestro rectangulo
    {
        
        windowLocation = 2;
    }
}

int Button::findWindow() const 
{
    return windowLocation;
}

Button::~Button()
{
}


