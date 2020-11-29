#include<iostream>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <fstream>
#include <SFML/Audio.hpp>

#include "Character.hh"
#include "GameObject.hh"
#include "Inputs.hh"
#include "Maze.hh"
#include "Tile.hh"
#include "ContactListener.hh"
#include "Score.hh"
#include "GUI/Button.hh"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define GAME_NAME "JUEGUITO"
#define ESCENARIO_AIRE_LIBRE "assets/sprites/Escenarios1.png"
#define OBJETOS_CAMPO "assets/sprites/OBJETOS_CAMPO.png"
#define TILES "assets/sprites/tiles2.png"
#define TILES2 "assets/sprites/tiles3.png"
#define SPRITE_SHEET_DOG "assets/sprites/dogSpriteFinal.png"
#define BG_MUSIC1 "assets/audio/musicaFondoAfuera.ogg"  
#define SPRITE_SCALE 4.f
#define PLAYER_MOVESPEED 20.0f
#define FPS 244
#define FONT "assets/fonts/NerkoOne-Regular.ttf"

int main()
{
    int changeWindow {0};
    //esto es la ventana de tu grafico
    sf::RenderWindow* mainMenuWindow = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);
    sf::RenderWindow* gameOverWindow = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);

    gameOverWindow->setVisible(false);
    window->setVisible(false);
    window->setActive(false);

    //DECLARAMOS EL SONIDO DEL JUEGO
    sf::SoundBuffer* soundBuffer{new sf::SoundBuffer()};
    sf::Sound* sound {new sf::Sound()};
    soundBuffer->loadFromFile(BG_MUSIC1);
    sound->setBuffer(*soundBuffer);
    
    
    Button* buttonCloseGame{new Button(640.f,360.f,150.f,50.f,0.5f, new sf::Color(255,0,0), new sf::Color(255,255,255), gameOverWindow)};
    Button* buttonStartGame{new Button(640.f,360.f,150.f,50.f,0.5f, new sf::Color(255,0,0), new sf::Color(255,255,255), mainMenuWindow)};

    //aqui vas a guardar los eventos dentro de la ventana, eje: teclado, mouse, etc.
    sf::Event event;
    sf::Event gameOverEvent;
    sf::Event mainMenuEvent;

    //CREAMOS EL SCORE
    Score* score{new Score(FONT, "Puntos: ", 32, new sf::Vector2f(25,5), new sf::Color(255,255,255), window)};

    //Camara para seguir al jugador
    sf::View* followPlayer {new sf::View()};
    followPlayer->setSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    followPlayer->setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    
    //creamos el minimapa utilizando views
    sf::View* miniMap {new sf::View()};
    miniMap->setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    miniMap->setViewport(sf::FloatRect(0.75f,0.f,0.25f,0.25f));
    
    //IMPLEMENTAR FONDO
    sf::Texture* gameOver {new sf::Texture()};
    gameOver->loadFromFile("assets/sprites/GameOver.png");

    sf::Texture* mainMenu {new sf::Texture()};
    mainMenu->loadFromFile("assets/sprites/Menu.png");

    //SPRITES-MINA
    sf::Texture* minaSprite {new sf::Texture()};
    minaSprite->loadFromFile("assets/sprites/SPRITES-MINA.png");

    sf::Sprite* fondoGameOver {new sf::Sprite(*gameOver)};

    sf::Sprite* fondoMainMenu {new sf::Sprite(*mainMenu)};
    
    sf::Sprite* mina {new sf::Sprite(*minaSprite)};
    

    //DECLARACIÓN DE LAS FISICAS
    b2Vec2* gravity{new b2Vec2(0.f, 0.f)};
    b2World* world{new b2World(*gravity)};


    sf::Clock* clock{new sf::Clock()};
    float deltaTime{};

    //ESTABLECEMOS LOS FRAMES A LOS QUE CORRERA EL PROGRAMA
    window->setFramerateLimit(FPS);

    //declaramos los inputs
    Inputs* inputs{new Inputs()};

    //DECLARACIÓN DE TEXTURAS DE IMAGENES 
    sf::Texture* tilesTexture {new sf::Texture()};
    tilesTexture->loadFromFile(TILES);

    sf::Texture* tilesTexture2 {new sf::Texture()};
    tilesTexture2->loadFromFile(TILES2);

    sf::Texture* spriteSheetDog {new sf::Texture()};
    spriteSheetDog->loadFromFile(SPRITE_SHEET_DOG);

    //DECLARACION DE LOS TEXTURE DE LOS ESCENARIOS 
    sf::Texture* esAireLibre {new sf::Texture()};
    esAireLibre->loadFromFile(ESCENARIO_AIRE_LIBRE);

    sf::Texture* objetosCampo {new sf::Texture()};
    objetosCampo->loadFromFile(OBJETOS_CAMPO);

    //PERSONAJE PRINCIPAL
    //Character* character{new Character(tilesTexture, 16*1, 16*5, 16, 16, SPRITE_SCALE, SPRITE_SCALE,
    //new b2Vec2(640, 360),b2BodyType::b2_dynamicBody,world, window)};

    Character* character{new Character(spriteSheetDog, 64*1, 64*1, 64, 64, 1.5, 1.5,
    new b2Vec2(800, 600),b2BodyType::b2_dynamicBody,world, window)};

    //Animaciones del personaje principal y asignación del tag
    character->SetAnimations(
        new Animation*[4]
        {
            new Animation(2,1,5, character->GetSprite(), 55.f),
            new Animation(1,1,8, character->GetSprite(), 40.f),
            new Animation(3,1,6, character->GetSprite(), 40.f),
            new Animation(4,1,6, character->GetSprite(), 40.f),
        }
    );
    character->SetTagName("player");

    sf::Sprite* floorSpike{new sf::Sprite(*tilesTexture2, * (new sf::IntRect(16*1, 16*11, 16,16)))};
    floorSpike->setScale(SPRITE_SCALE, SPRITE_SCALE);
    floorSpike->setPosition(16*16,16*20);

    /*sf::Sprite* trampaPerro{new sf::Sprite(*spriteSheetDog, * (new sf::IntRect(64*1, 64*11, 64,64)))};
    trampaPerro->setPosition(16*20,16*25); */

    Animation* spikeAnimation {new Animation(11, 1,4, floorSpike, 300.f)};
    
    
    //GAME OBJECTSS
        GameObject* treasure{new GameObject(tilesTexture2, 16*19, 16*19,16,16,
        SPRITE_SCALE, SPRITE_SCALE, new b2Vec2(400,400),b2BodyType::b2_staticBody ,world, window)};
        treasure->SetTagName("treasure");

        GameObject* treee{new GameObject(objetosCampo, 64*3, 64*2,64*1,64*1,
        1, 1, new b2Vec2(2000,1000),b2BodyType::b2_staticBody ,world, window)};

    //CARNES PARA EL PERRO
        GameObject* meat{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        1.5, 1.5, new b2Vec2(1000,1000),b2BodyType::b2_staticBody ,world, window)};
        meat->SetTagName("meat");
        
        GameObject* meat1{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        1.5, 1.5, new b2Vec2(900,500),b2BodyType::b2_staticBody ,world, window)};
        meat1->SetTagName("meat");

        GameObject* meat2{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        1.5, 1.5, new b2Vec2(1828,766),b2BodyType::b2_staticBody ,world, window)};
        meat2->SetTagName("meat");

        GameObject* meat3{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        1.5, 1.5, new b2Vec2(2304,384),b2BodyType::b2_staticBody ,world, window)};
        meat3->SetTagName("meat");

        GameObject* meat4{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        1.5, 1.5, new b2Vec2(2057,1390),b2BodyType::b2_staticBody ,world, window)};
        meat4->SetTagName("meat");
    //TRAMPAS PARA EL PERRO
        GameObject* trampaPerro{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(1300,600),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro->SetTagName("enemy");

        GameObject* trampaPerro1{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(1576,706),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro1->SetTagName("enemy");

        GameObject* trampaPerro2{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(1839,873),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro2->SetTagName("enemy");

        GameObject* trampaPerro3{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(2350,560),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro3->SetTagName("enemy");

        GameObject* trampaPerro4{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(1247,1247),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro4->SetTagName("enemy");

        GameObject* tree{new GameObject(esAireLibre, 128*1.5, 128*1.5,128,128,
        2, 2, new b2Vec2(100,600),b2BodyType::b2_staticBody ,world, window)};

        GameObject* tree1{new GameObject(esAireLibre, 128*1.5, 128*1.5,128,128,
        2, 2, new b2Vec2(3750,1400),b2BodyType::b2_staticBody ,world, window)};

        GameObject* tronco{new GameObject(esAireLibre, 128*1.5, 64*5,128,64,
        2, 2, new b2Vec2(1000,900),b2BodyType::b2_staticBody ,world, window)};

    //CREAR VECTOR CON LOS GAME OBJECT
        std::vector<GameObject*>* items{new std::vector<GameObject*>()};
        items->push_back(treasure);
        items->push_back(trampaPerro);
        items->push_back(tree);
        items->push_back(tree1);
        items->push_back(treee);
        items->push_back(tronco);
    //AGREGAR CARNES AL MAPA
        items->push_back(meat); 
        items->push_back(meat1);
        items->push_back(meat2);
        items->push_back(meat3);
        items->push_back(meat4);
    //AGREGAR TRAMPAS PARA EL PERRO     
        items->push_back(trampaPerro);
        items->push_back(trampaPerro1);
        items->push_back(trampaPerro2);
        items->push_back(trampaPerro3);
        items->push_back(trampaPerro4);
    //ANIMACIONES TRAMPA
    Animation* trampaPerroAnimation {new Animation(11, 1,10, trampaPerro->GetSprite(), 50.f)}; 

    //DEFINIR EL CONTACTLISTENER PARA LAS COLLISIONES
    ContactListener* contactListener{new ContactListener(score, items)};

    world->SetContactListener(contactListener);

    //Definimos el maze
    unsigned int N{10}, M{13};
    Maze* maze1{new Maze(N,M,1,64,minaSprite,"assets/mazes/maze1.txt", world)};
    Maze* mapAireLibre{new Maze(N*2, M*6, 1.5, 64, esAireLibre, "assets/mazes/mapAireLibre.txt", world)};
    Maze*& currentMaze{*&mapAireLibre};

    // WINDOW MAIN MENU
    while (mainMenuWindow->isOpen())
            {
                while (mainMenuWindow->pollEvent(mainMenuEvent))
                {
                    if (mainMenuEvent.type == sf::Event::Closed)
                    {
                        mainMenuWindow->close();
                        return 0;
                    }
                }
                mainMenuWindow->clear(*(new sf::Color(0, 255, 0, 255)));//limpiar la pantalla  

                
                
                mainMenuWindow->draw(*fondoMainMenu);
                buttonStartGame->UpdateStart();

                if (buttonStartGame->findWindow() == 2)
                {
                    mainMenuWindow->setVisible(false);
                    mainMenuWindow->setActive(false);
                    window->setVisible(true);
                    window->setActive(true);

                    sound->setVolume(0.5f);
                    sound->setLoop(true);
                    sound->play();








                    //WINDOW JUEGO PRINCIPAL
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

                        switch (contactListener->GetSceneIndex())
                        {
                            case 0:
                                currentMaze = mapAireLibre;
                                break;
                            case 1:
                                currentMaze = maze1;
                                break;
                            default:
                                currentMaze = mapAireLibre;
                                break;
                        }

                        Vec2* keyboardAxis {inputs->GetKeyboardAxis()};
                        Vec2* joystickAxis {inputs->GetJoystickAxis()};



                        if(sf::Joystick::isConnected(0))
                        {
                            character->Move(new b2Vec2(joystickAxis->x * deltaTime * PLAYER_MOVESPEED, joystickAxis->y * deltaTime * PLAYER_MOVESPEED));
                            character->FlipSpriteX(joystickAxis->x);
                            //APLICAMOS ANIMACIONES AL PERSONAJE
                            if (std::abs(joystickAxis->x) > 0 && std::abs(joystickAxis->y) == 0)
                            {
                                character->GetAnimation(1)->Play(deltaTime);
                            }
                            else if(joystickAxis->x == 0 && joystickAxis->y == 0)
                            {
                                character->GetAnimation(0)->Play(deltaTime);
                            }
                            if (joystickAxis->y > 0)
                            {
                                character->GetAnimation(3)->Play(deltaTime);
                                
                            } else if (joystickAxis->y < 0)
                            {
                                character->GetAnimation(2)->Play(deltaTime);
                            }
                        }
                        else
                        {
                            character->Move(new b2Vec2(keyboardAxis->x * deltaTime * PLAYER_MOVESPEED, keyboardAxis->y * deltaTime * PLAYER_MOVESPEED));
                            character->FlipSpriteX(keyboardAxis->x);
                            //APLICAMOS ANIMACIONES AL PERSONAJE CON TECLADO
                            if (std::abs(keyboardAxis->x) > 0 && std::abs(keyboardAxis->y) == 0)
                            {
                                character->GetAnimation(1)->Play(deltaTime);
                            }
                            else if(keyboardAxis->x == 0 && keyboardAxis->y == 0)
                            {
                                character->GetAnimation(0)->Play(deltaTime);
                            }
                            if (keyboardAxis->y > 0)
                            {
                                character->GetAnimation(3)->Play(deltaTime);
                                
                            } else if (keyboardAxis->y < 0)
                            {
                                character->GetAnimation(2)->Play(deltaTime);
                            }


                        }

                        followPlayer->setCenter(character->GetCharacterPosition().x,character->GetCharacterPosition().y);

                        //miniMap->setCenter(character->GetCharacterPosition().x, 360.f);
                        miniMap->setCenter(followPlayer->getCenter().x, followPlayer->getCenter().y);


                        //SE LE RESTA LA MITAD DE LAS DIMENSIONES PARA QUE EL PERSONAJE QUEDE CENTRADO CON LA IMAGEN EN TODO MOMENTO
                        //fondo->setPosition(character->GetCharacterPosition().x - 640.f, character->GetCharacterPosition().y - 360.f);
                        

                        
                        //std::cout << character->GetCharacterPosition().x << std::endl;
                        //std::cout << character->GetCharacterPosition().y << std::endl;


                        window->clear(*(new sf::Color(0, 100, 0, 255)));//limpiar la pantalla     

                        //CAMARA DEL JUEGO PRINCIPAL
                        window->setView(*followPlayer);

                        //window->draw(*fondo); 

                        for (auto& tile: *mapAireLibre->GetContainer())
                        {
                            window->draw(*tile->GetSprite());
                        }
                        int x=0;
                        for(auto& item : *items)
                        {
                            item->Update();
                        }


                        trampaPerroAnimation->Play(deltaTime);
                        
                        window->draw(*floorSpike);
                        //window->draw(*trampaPerro);
                        character->Update();
                        
                        score->update(followPlayer->getCenter().x -630, followPlayer->getCenter().y -360);

                        //SI PIERDE
                        if(contactListener->IsGameOver())
                        {
                            std::cout << "Perdiste" << std::endl;
                        }
                        contactListener->ResetGameOver(false);

                        if (score->GetPoints()>20)
                        {
                            std::cout << "Perdiste" << std::endl;
                            window->setVisible(false);
                            window->setActive(false);
                            gameOverWindow->setVisible(true);
                            sound->stop();





                            //WINDOW DEL GAME OVER
                            while (gameOverWindow->isOpen())
                            {
                                while (gameOverWindow->pollEvent(gameOverEvent))
                                {
                                    if (gameOverEvent.type == sf::Event::Closed)
                                    {
                                        gameOverWindow->close();
                                        return 0;
                                    }
                                }
                                gameOverWindow->clear(*(new sf::Color(0, 255, 0, 255)));//limpiar la pantalla  

                                
                                buttonCloseGame->Update();
                                gameOverWindow->draw(*fondoGameOver);
                                

                                if (buttonCloseGame->findWindow() == 3)
                                {
                                    return 0;
                                }
                                gameOverWindow->display();
                            }


                            
                        } 

                        

                        //VISTA DEL MINIMAPA
                        
                        window->setView(*miniMap);

                        //window->draw(*fondo); 

                        for (auto& tile: *mapAireLibre->GetContainer())
                        {
                            window->draw(*tile->GetSprite());
                        }

                        for(auto& item : *items)
                        {
                            item->Update();
                        }

                        character->Update();

                        window->display(); //mostrar en pantalla lo que se va dibujar

                        //CONFIGURAR DELTA TIME
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





                
                mainMenuWindow->display();
            }
        
    
    
}