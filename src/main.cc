#include<iostream>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <fstream>

#include "Character.hh"
#include "GameObject.hh"
#include "Inputs.hh"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define GAME_NAME "JUEGUITO"
#define TILES "assets/sprites/tiles2.png"
#define TILES2 "assets/sprites/tiles3.png"
#define SPRITE_SCALE 4.f
#define PLAYER_MOVESPEED 3.0f
#define FPS 120



int main()
{

    float jumpF = 400.f;
    float speedValue=0.f;

    //esto es la ventana de tu grafico
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);

    
    //aqui vas a guardar los eventos dentro de la ventana, eje: teclado, mouse, etc.
    sf::Event event;

    //Camara para seguir al jugador
    sf::View* followPlayer {new sf::View()};
    followPlayer->setSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    //IMPLEMENTAR FONDO
    sf::Texture* fondoE1 {new sf::Texture()};
    fondoE1->loadFromFile("assets/sprites/FondoE1.png");

    //SPRITES-MINA
    sf::Texture* minaSprite {new sf::Texture()};
    minaSprite->loadFromFile("assets/sprites/SPRITES-MINA.png");


    sf::Sprite* fondo {new sf::Sprite(*fondoE1)};
    sf::Sprite* mina {new sf::Sprite(*minaSprite)};
    
    fondo->scale(SPRITE_SCALE, 1);
    

    //DECLARACIÓN DE LAS FISICAS
    b2Vec2* gravity{new b2Vec2(0.f, 0.f)};
    b2World* world{new b2World(*gravity)};


    sf::Clock* clock{new sf::Clock()};
    float deltaTime{};

    window->setFramerateLimit(FPS);

    Inputs* inputs{new Inputs()};

    sf::Texture* tileTexture {new sf::Texture()};
    tileTexture->loadFromFile(TILES);

    sf::Texture* tileTexture2 {new sf::Texture()};
    tileTexture2->loadFromFile(TILES2);

    //PERSONAJE PRINCIPAL
    Character* character{new Character(tileTexture, 16*1, 16*5, 16, 16, SPRITE_SCALE, SPRITE_SCALE,
    new b2Vec2(640, 360),b2BodyType::b2_dynamicBody,world, window)};

    //Animaciones del personaje principal
    character->SetAnimations(
        new Animation*[2]
        {
            new Animation(5,0,5, character->GetSprite(), 40.f),
            new Animation(6,0,5, character->GetSprite(), 80.f)
        }
    );


    sf::Sprite* floorSpike{new sf::Sprite(*tileTexture2, * (new sf::IntRect(16*1, 16*11, 16,16)))};
    floorSpike->setScale(SPRITE_SCALE, SPRITE_SCALE);
    floorSpike->setPosition(16*16,16*20);

    

    //esto es el loop principal, mientras la ventana este abierta, esto se va ejecutar.
    while (window->isOpen())
    {

        //mientras se esten ejecutando eventos dentro de la ventana, esto se va repetir eje: teclado, joystick, mouse, etc
        while (window->pollEvent(event))
        {
            //si el evento fue la acción de cerrar la ventana, entonces termina la aplicación.
            if(event.type == sf::Event::Closed)
            {
                window->close();
            }
        }

        Vec2* keyboardAxis {inputs->GetKeyboardAxis()};
        Vec2* joystickAxis {inputs->GetJoystickAxis()};
        sf::Vector2f velocity(sf::Vector2f(0,0));
        speedValue=  jumpF* deltaTime * PLAYER_MOVESPEED;

        if(sf::Joystick::isConnected(0))
        {
            character->Move(new b2Vec2(joystickAxis->x * deltaTime * PLAYER_MOVESPEED, joystickAxis->y * deltaTime * PLAYER_MOVESPEED));
            character->FlipSpriteX(joystickAxis->x);
            //APLICAMOS ANIMACIONES AL PERSONAJE
            if (std::abs(joystickAxis->x) > 0 || std::abs (joystickAxis->y) > 0)
            {
                character->GetAnimation(1)->Play(deltaTime);
            }
            else
            {
                character->GetAnimation(0)->Play(deltaTime);
            }
        }
        else
        {
            character->Move(new b2Vec2(keyboardAxis->x * deltaTime * PLAYER_MOVESPEED, keyboardAxis->y * deltaTime * PLAYER_MOVESPEED));
            character->FlipSpriteX(keyboardAxis->x);
            //APLICAMOS ANIMACIONES AL PERSONAJE CON TECLADO
            if (std::abs(keyboardAxis->x) > 0 || std::abs (keyboardAxis->y) > 0)
            {
                character->GetAnimation(1)->Play(deltaTime);
            }
            else
            {
                character->GetAnimation(0)->Play(deltaTime);
            }


            if(inputs->GetIsJumping())
            {
                //std::cout << "funciona" << std::endl;
                velocity.y = -speedValue;
                character->Move(new b2Vec2(0, velocity.y));
            }
            if (character->GetCharacterPosition().y < 100.f)
            {
                std::cout << "putamadre" << std::endl;
            }


            //character->Move(new b2Vec2(0, 9.f* deltaTime));
        }

          followPlayer->setCenter(character->GetCharacterPosition().x, 360.f);

        //SE LE RESTA LA MITAD DE LAS DIMENSIONES PARA QUE EL PERSONAJE QUEDE CENTRADO CON LA IMAGEN EN TODO MOMENTO
         //fondo->setPosition(character->GetCharacterPosition().x - 640.f, character->GetCharacterPosition().y - 360.f);

        
        //std::cout << character->GetCharacterPosition().x << std::endl;
        std::cout << character->GetCharacterPosition().y << std::endl;


        window->clear(*(new sf::Color(0, 100, 0, 255)));//limpiar la pantalla      

        window->setView(*followPlayer);
        window->draw(*floorSpike);
        //window->draw(*fondo);
        window->draw(*mina);
        character->Update();
        window->display(); //mostrar en pantalla lo que se va dibujar

        sf::Time timeElapsed = clock->getElapsedTime();
        deltaTime = timeElapsed.asMilliseconds();
        world->ClearForces();
        world->Step(1.f/100*deltaTime, 8, 8);
        clock->restart();

        delete keyboardAxis;
        delete joystickAxis;
    }
    return 0;
}