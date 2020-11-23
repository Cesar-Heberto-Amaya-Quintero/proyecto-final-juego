#pragma once
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Tile.hh"
#include <box2d/box2d.h>

class Maze
{
    private:
        unsigned int N{}, M{};
        const char* path{};
        std::fstream* reader{new std::fstream()};
        char** tiles{};
        float scale, cropSize;
        sf::Texture* texture; 
        std::vector<Tile*>* container{new std::vector<Tile*>{}};
        b2World* world;
    public:
        Maze(unsigned int, unsigned int,float,float ,sf::Texture*&, const char*, b2World*&);
        void AllocateMemory();
        void FillArray();
        char** GetTiles() const;
        ~Maze();
        void Generate();
        std::vector<Tile*>* GetContainer() const;
};