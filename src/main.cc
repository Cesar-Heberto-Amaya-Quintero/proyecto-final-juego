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
#define GAME_NAME "Wild Doggie"
#define OBJETOS_AIRE_LIBRE "assets/sprites/Escenarios.png"
#define ESCENARIO_AIRE_LIBRE "assets/sprites/Escenarios1.png"
#define OBJETOS_CAMPO "assets/sprites/OBJETOS_CAMPO.png"
#define TILES "assets/sprites/tiles2.png"
#define TILES2 "assets/sprites/tiles3.png"
#define SPRITE_SHEET_DOG "assets/sprites/dogSpriteFinal.png"
#define ICON "assets/sprites/IconoJuego.png"
#define BG_MUSIC1 "assets/audio/musicaFondoAfuera.ogg"  
#define BG_MENU "assets/audio/menu.ogg" 
#define BG_LOSE "assets/audio/lose.ogg" 
#define BG_WIN "assets/audio/win.ogg" 
#define SPRITE_SCALE 4.f
#define PLAYER_MOVESPEED 20.0f
#define FPS 244
#define FONT "assets/fonts/NerkoOne-Regular.ttf"

int main()
{
    //esto es la ventana de tu grafico
    sf::RenderWindow* mainMenuWindow = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);
    sf::RenderWindow* finalWindow = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_NAME);

    sf::Image* iconTexture {new sf::Image()};
    iconTexture->loadFromFile(ICON);
    window->setIcon(64,64, iconTexture->getPixelsPtr());
    mainMenuWindow->setIcon(64,64, iconTexture->getPixelsPtr());
    finalWindow->setIcon(64,64, iconTexture->getPixelsPtr());

    finalWindow->setVisible(false);
    window->setVisible(false);
    window->setActive(false);

    //DECLARAMOS LOS SONIDO DEL JUEGO
    sf::SoundBuffer* soundBuffer{new sf::SoundBuffer()};
    sf::Sound* sound {new sf::Sound()};
    soundBuffer->loadFromFile(BG_MUSIC1);
    sound->setBuffer(*soundBuffer);

    sf::SoundBuffer* soundBufferMenu{new sf::SoundBuffer()};
    sf::Sound* soundMenu {new sf::Sound()};
    soundBufferMenu->loadFromFile(BG_MENU);
    soundMenu->setBuffer(*soundBufferMenu);

    sf::SoundBuffer* soundBufferLose{new sf::SoundBuffer()};
    sf::Sound* soundLose {new sf::Sound()};
    soundBufferLose->loadFromFile(BG_LOSE);
    soundLose->setBuffer(*soundBufferLose);
    
    sf::SoundBuffer* soundBufferWin{new sf::SoundBuffer()};
    sf::Sound* soundWin {new sf::Sound()};
    soundBufferWin->loadFromFile(BG_WIN);
    soundWin->setBuffer(*soundBufferWin);
    
    //DECLARAMOS LOS BOTONES
    Button* buttonCloseGame{new Button(1100.f,80.f,70.f,70.f,0.5f, new sf::Color(255,0,0), new sf::Color(255,255,255), finalWindow)};
    Button* buttonStartGame{new Button(275.f,440.f,260.f,115.f,0.5f, new sf::Color(255,0,0), new sf::Color(255,255,255), mainMenuWindow)};
    Button* buttonMenuCloseGame{new Button(1100.f,80.f,70.f,70.f,0.5f, new sf::Color(255,0,0), new sf::Color(255,255,255), mainMenuWindow)};

    //aqui vas a guardar los eventos dentro de la ventana, eje: teclado, mouse, etc.
    sf::Event event;
    sf::Event finalEvent;
    sf::Event mainMenuEvent;

    //CREAMOS EL SCORE
    Score* score{new Score(FONT, "Meat: ", 32, new sf::Vector2f(25,5), new sf::Color(255,255,255), window)};

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

    sf::Texture* gameWin {new sf::Texture()};
    gameWin->loadFromFile("assets/sprites/GameWin.png");

    sf::Texture* mainMenu {new sf::Texture()};
    mainMenu->loadFromFile("assets/sprites/Menu.png");

    //SPRITES-MINA
    sf::Texture* minaSprite {new sf::Texture()};
    minaSprite->loadFromFile("assets/sprites/SPRITES-MINA.png");

    sf::Sprite* fondoGameOver {new sf::Sprite(*gameOver)};

    sf::Sprite* fondoGameWin {new sf::Sprite(*gameWin)};

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

    sf::Texture* obAireLibre {new sf::Texture()};
    obAireLibre->loadFromFile(OBJETOS_AIRE_LIBRE);

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


    //CARNES PARA EL PERRO
        
        GameObject* meat1{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        0.75, 0.75, new b2Vec2(1000,1000),b2BodyType::b2_staticBody ,world, window)};
        meat1->SetTagName("meat");

        GameObject* meat2{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        0.75, 0.75, new b2Vec2(1828,766),b2BodyType::b2_staticBody ,world, window)};
        meat2->SetTagName("meat");

        GameObject* meat3{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        0.75, 0.75, new b2Vec2(2304,400),b2BodyType::b2_staticBody ,world, window)};
        meat3->SetTagName("meat");

        GameObject* meat4{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        0.75, 0.75, new b2Vec2(1305,1470),b2BodyType::b2_staticBody ,world, window)};
        meat4->SetTagName("meat");

        GameObject* meat5{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        0.75, 0.75, new b2Vec2(3574,1510),b2BodyType::b2_staticBody ,world, window)};
        meat5->SetTagName("meat");

        GameObject* meat6{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        0.75, 0.75, new b2Vec2(2606,1498),b2BodyType::b2_staticBody ,world, window)};
        meat6->SetTagName("meat");

        GameObject* meat7{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        0.75, 0.75, new b2Vec2(4773,1000),b2BodyType::b2_staticBody ,world, window)};
        meat7->SetTagName("meat");

        GameObject* meat8{new GameObject(spriteSheetDog, 64*1, 64*7,64,64,
        0.75, 0.75, new b2Vec2(2535,925),b2BodyType::b2_staticBody ,world, window)};
        meat8->SetTagName("meat");

        GameObject* meat9{new GameObject(spriteSheetDog, 64*1, 64*9,64,64,
        0.75, 0.75, new b2Vec2(3493,644),b2BodyType::b2_staticBody ,world, window)};
        meat9->SetTagName("meat");

        GameObject* meat10{new GameObject(spriteSheetDog, 64*1, 64*9,64,64,
        0.75, 0.75, new b2Vec2(4735,440),b2BodyType::b2_staticBody ,world, window)};
        meat10->SetTagName("meat");

        GameObject* meat11{new GameObject(spriteSheetDog, 64*1, 64*9,64,64,
        0.75, 0.75, new b2Vec2(4180,549),b2BodyType::b2_staticBody ,world, window)};
        meat11->SetTagName("meat");

        GameObject* meat12{new GameObject(spriteSheetDog, 64*1, 64*8,64,64,
        0.75, 0.75, new b2Vec2(5567,414),b2BodyType::b2_staticBody ,world, window)};
        meat12->SetTagName("meat");

        GameObject* meat13{new GameObject(spriteSheetDog, 64*1, 64*8,64,64,
        0.75, 0.75, new b2Vec2(5545,1545),b2BodyType::b2_staticBody ,world, window)};
        meat13->SetTagName("meat");

        GameObject* meat14{new GameObject(spriteSheetDog, 64*1, 64*8,64,64,
        0.75, 0.75, new b2Vec2(5962,1280),b2BodyType::b2_staticBody ,world, window)};
        meat14->SetTagName("meat");

        GameObject* meat15{new GameObject(spriteSheetDog, 64*1, 64*8,64,64,
        0.75, 0.75, new b2Vec2(6568,422),b2BodyType::b2_staticBody ,world, window)};
        meat15->SetTagName("meat");
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
        1, 1, new b2Vec2(2363,580),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro3->SetTagName("enemy");

        GameObject* trampaPerro4{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(1247,1247),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro4->SetTagName("enemy");

        GameObject* trampaPerro5{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(2496,1445),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro5->SetTagName("enemy");

        GameObject* trampaPerro6{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(2496,1545),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro6->SetTagName("enemy");

        GameObject* trampaPerro7{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(2875,1439),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro7->SetTagName("enemy");

        GameObject* trampaPerro8{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(4127,1152),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro8->SetTagName("enemy");

        GameObject* trampaPerro9{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(4800,1535),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro9->SetTagName("enemy");

        GameObject* trampaPerro10{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(4320,1229),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro10->SetTagName("enemy");

        GameObject* trampaPerro11{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(4420,863),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro11->SetTagName("enemy");

        GameObject* trampaPerro12{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(3199,975),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro12->SetTagName("enemy");

        GameObject* trampaPerro13{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(2363,510),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro13->SetTagName("enemy");

        GameObject* trampaPerro14{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(2708,426),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro14->SetTagName("enemy");

        GameObject* trampaPerro15{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(3654,680),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro15->SetTagName("enemy");

        GameObject* trampaPerro16{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(3654,405),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro16->SetTagName("enemy");

        GameObject* trampaPerro17{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(4200,680),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro17->SetTagName("enemy");

        GameObject* trampaPerro18{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(4200,395),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro18->SetTagName("enemy");

        GameObject* trampaPerro19{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(4747,570),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro19->SetTagName("enemy");

        GameObject* trampaPerro20{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(1676,1492),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro20->SetTagName("enemy");

        GameObject* trampaPerro21{new GameObject(spriteSheetDog, 64*1, 64*11,64,64,
        1, 1, new b2Vec2(2208,808),b2BodyType::b2_staticBody ,world, window)};
        trampaPerro21->SetTagName("enemy");

        //PICOS
        GameObject* pico1{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(5760,672),b2BodyType::b2_staticBody ,world, window)};
        pico1->SetTagName("enemy");

        GameObject* pico2{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(5376,1020),b2BodyType::b2_staticBody ,world, window)};
        pico2->SetTagName("enemy");

        GameObject* pico3{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(5568,1322),b2BodyType::b2_staticBody ,world, window)};
        pico3->SetTagName("enemy");

        GameObject* pico4{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(6050,864),b2BodyType::b2_staticBody ,world, window)};
        pico4->SetTagName("enemy");

        GameObject* pico5{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(6434,672),b2BodyType::b2_staticBody ,world, window)};
        pico5->SetTagName("enemy");

        GameObject* pico6{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(6720,934),b2BodyType::b2_staticBody ,world, window)};
        pico6->SetTagName("enemy");

        GameObject* pico7{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(6148,1344),b2BodyType::b2_staticBody ,world, window)};
        pico7->SetTagName("enemy");

        GameObject* pico8{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(5856,1152),b2BodyType::b2_staticBody ,world, window)};
        pico8->SetTagName("enemy");

        GameObject* pico9{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(6392,1040),b2BodyType::b2_staticBody ,world, window)};
        pico9->SetTagName("enemy");

        GameObject* pico10{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(6152,864),b2BodyType::b2_staticBody ,world, window)};
        pico10->SetTagName("enemy");

        GameObject* pico11{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(5190,480),b2BodyType::b2_staticBody ,world, window)};
        pico11->SetTagName("enemy");

        GameObject* pico12{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(5568,564),b2BodyType::b2_staticBody ,world, window)};
        pico12->SetTagName("enemy");

        GameObject* pico13{new GameObject(esAireLibre, 64*27, 64*7,64,64,
        1.5, 1.5, new b2Vec2(6720,578),b2BodyType::b2_staticBody ,world, window)};
        pico13->SetTagName("enemy");
        

        GameObject* tree{new GameObject(obAireLibre, 128*1.5, 128*1.5,128,128,
        2, 2, new b2Vec2(100,600),b2BodyType::b2_staticBody ,world, window)};

        GameObject* tree1{new GameObject(obAireLibre, 128*1.5, 128*1.5,128,128,
        2, 2, new b2Vec2(3750,1450),b2BodyType::b2_staticBody ,world, window)};


    //CREAR VECTOR CON LOS GAME OBJECT
        std::vector<GameObject*>* items{new std::vector<GameObject*>()};
        items->push_back(tree);
        items->push_back(tree1);
    //AGREGAR CARNES AL MAPA
        items->push_back(meat1);
        items->push_back(meat2);
        items->push_back(meat3);
        items->push_back(meat4);
        items->push_back(meat5);
        items->push_back(meat6);
        items->push_back(meat7);
        items->push_back(meat8);
        items->push_back(meat9);
        items->push_back(meat10);
        items->push_back(meat11);
        items->push_back(meat12);
        items->push_back(meat13);
        items->push_back(meat14);
        items->push_back(meat15);
    //AGREGAR TRAMPAS PARA EL PERRO     
        items->push_back(trampaPerro);
        items->push_back(trampaPerro1);
        items->push_back(trampaPerro2);
        items->push_back(trampaPerro3);
        items->push_back(trampaPerro4);
        items->push_back(trampaPerro5);
        items->push_back(trampaPerro6);
        items->push_back(trampaPerro7);
        items->push_back(trampaPerro8);
        items->push_back(trampaPerro9);
        items->push_back(trampaPerro10);
        items->push_back(trampaPerro11);
        items->push_back(trampaPerro12);
        items->push_back(trampaPerro13);
        items->push_back(trampaPerro14);
        items->push_back(trampaPerro15);
        items->push_back(trampaPerro16);
        items->push_back(trampaPerro17);
        items->push_back(trampaPerro18);
        items->push_back(trampaPerro19);
        items->push_back(trampaPerro20);
        items->push_back(trampaPerro21);
    // PICOS
        items->push_back(pico1);
        items->push_back(pico2);
        items->push_back(pico3);
        items->push_back(pico4);
        items->push_back(pico5);
        items->push_back(pico6);
        items->push_back(pico7);
        items->push_back(pico8);
        items->push_back(pico9);
        items->push_back(pico10);
        items->push_back(pico11);
        items->push_back(pico12);
        items->push_back(pico13);

    //DEFINIR EL CONTACTLISTENER PARA LAS COLLISIONES
    ContactListener* contactListener{new ContactListener(score, items)};

    world->SetContactListener(contactListener);

    //Definimos el maze
    unsigned int N{10}, M{13};
    Maze* maze1{new Maze(N,M,1,64,minaSprite,"assets/mazes/maze1.txt", world)};
    Maze* mapAireLibre{new Maze(N*2, M*6, 1.5, 64, esAireLibre, "assets/mazes/mapAireLibre.txt", world)};
    Maze*& currentMaze{*&mapAireLibre};

    // WINDOW MAIN MENU
    soundMenu->setVolume(5.0f);
    soundMenu->setLoop(true);
    soundMenu->play();
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

                
                buttonStartGame->UpdateStart();
                buttonMenuCloseGame->Update();
                mainMenuWindow->draw(*fondoMainMenu);
                
                if (buttonMenuCloseGame->findWindow() == 3)
                    {
                        return 0;
                    }

                if (buttonStartGame->findWindow() == 2)
                {
                    soundMenu->stop();
                    mainMenuWindow->setVisible(false);
                    mainMenuWindow->setActive(false);
                    window->setVisible(true);
                    window->setActive(true);

                    sound->setVolume(5.f);
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

                        miniMap->setCenter(followPlayer->getCenter().x, followPlayer->getCenter().y);
                        

                        
                        std::cout << character->GetCharacterPosition().x << std::endl;
                        std::cout << character->GetCharacterPosition().y << std::endl;


                        window->clear(*(new sf::Color(0, 100, 0, 255)));//limpiar la pantalla     

                        //CAMARA DEL JUEGO PRINCIPAL
                        window->setView(*followPlayer);


                        for (auto& tile: *mapAireLibre->GetContainer())
                        {
                            window->draw(*tile->GetSprite());
                        }
                        int x=0;
                        for(auto& item : *items)
                        {
                            item->Update();
                        }


                        //window->draw(*trampaPerro);
                        character->Update();
                        
                        score->update(followPlayer->getCenter().x -630, followPlayer->getCenter().y -360);

                        //SI GANA

                        if (score->GetPoints()==15)
                        {
                            std::cout << "Ganaste" << std::endl;
                            window->setVisible(false);
                            window->setActive(false);
                            finalWindow->setVisible(true);
                            sound->stop();

                            soundWin->setVolume(5.0f);
                            soundWin->setLoop(true);
                            soundWin->play();




                            //WINDOW DEL win
                            while (finalWindow->isOpen())
                            {
                                while (finalWindow->pollEvent(finalEvent))
                                {
                                    if (finalEvent.type == sf::Event::Closed)
                                    {
                                        finalWindow->close();
                                        return 0;
                                    }
                                }
                                finalWindow->clear(*(new sf::Color(0, 255, 0, 255)));//limpiar la pantalla  

                                
                                buttonCloseGame->Update();
                                finalWindow->draw(*fondoGameWin);
                                

                                if (buttonCloseGame->findWindow() == 3)
                                {
                                    return 0;
                                }
                                finalWindow->display();
                            }


                            
                        } 

                        //SI PIERDE
                        if (contactListener->IsGameOver())
                        {
                            std::cout << "Perdiste" << std::endl;
                            window->setVisible(false);
                            window->setActive(false);
                            finalWindow->setVisible(true);
                            sound->stop();

                            soundLose->setVolume(5.0f);
                            soundLose->setLoop(true);
                            soundLose->play();



                            //WINDOW DEL GAME OVER
                            while (finalWindow->isOpen())
                            {
                                while (finalWindow->pollEvent(finalEvent))
                                {
                                    if (finalEvent.type == sf::Event::Closed)
                                    {
                                        finalWindow->close();
                                        return 0;
                                    }
                                }
                                finalWindow->clear(*(new sf::Color(0, 255, 0, 255)));//limpiar la pantalla  

                                
                                buttonCloseGame->Update();
                                finalWindow->draw(*fondoGameOver);
                                

                                if (buttonCloseGame->findWindow() == 3)
                                {
                                    return 0;
                                }
                                finalWindow->display();
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