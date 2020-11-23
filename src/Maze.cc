#include "Maze.hh"

Maze::Maze(unsigned int N, unsigned int M, float scale, float cropSize, sf::Texture *&texture, const char *path, b2World *&world)
{
    this->N = N;
    this->M = M;
    this->scale = scale;
    this->cropSize = cropSize;
    this->texture = texture;
    this->path = path;
    this->world = world;
    tiles = new char *[N];
    reader->open(path);
    AllocateMemory();
    FillArray();
    Generate();
}

void Maze::AllocateMemory()
{
    for (int i{}; i < N; i++)
    {
        tiles[i] = new char[M];
    }
}

void Maze::FillArray()
{
    for (int i{}; i < N; i++)
    {
        for (int j{}; j < M; j++)
        {
            *reader >> tiles[i][j];
        }
    }
}

void Maze::Generate()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {

            char &tile{tiles[i][j]};

            //tileBaseWidht vale 64
            //La letra # = pisoesquinaIzquierda, - = piso en medio, $= piso esquina derecha, g=Ground1, f=ground2, s= wallWater, c=floorWater
            // e= wallFire t=floorFire d=floorStairs x=floorBroken
            switch (tile)
            {
            //ESCENARIO CAMPO
            //PRIMERA FILA
            case 'a':
                container->push_back(new Tile(cropSize * 1, cropSize * 1, scale, cropSize, texture));
                break;
            case 'b':
                container->push_back(new Tile(cropSize * 2, cropSize * 1, scale, cropSize, texture));
                break;
            case 'c':
                container->push_back(new Tile(cropSize * 3, cropSize * 1, scale, cropSize, texture));
                break;
            case 'd':
                container->push_back(new Tile(cropSize * 4, cropSize * 1, scale, cropSize, texture));
                break;
            case 'e':
                container->push_back(new Tile(cropSize * 5, cropSize * 1, scale, cropSize, texture));
                break;
            case 'f':
                container->push_back(new Tile(cropSize * 6, cropSize * 1, scale, cropSize, texture));
                break;
            //SEGUNDA FILA
            case 'g':
                container->push_back(new Tile(cropSize * 1, cropSize * 2, scale, cropSize, texture));
                break;
            case 'h':
                container->push_back(new Tile(cropSize * 2, cropSize * 2, scale, cropSize, texture));
                break;
            case 'i':
                container->push_back(new Tile(cropSize * 3, cropSize * 2, scale, cropSize, texture));
                break;
            case 'j':
                container->push_back(new Tile(cropSize * 4, cropSize * 2, scale, cropSize, texture));
                break;
            case 'k':
                container->push_back(new Tile(cropSize * 5, cropSize * 2, scale, cropSize, texture));
                break;
            case 'l':
                container->push_back(new Tile(cropSize * 6, cropSize * 2, scale, cropSize, texture));
                break;
            case 'm':
                container->push_back(new Tile(cropSize * 1, cropSize * 6, scale, cropSize, texture));
                break;
            //ESCENARIO PLAYA
            //PRIMERA FILA
            //ARENA
            case 'A':
                container->push_back(new Tile(cropSize * 9, cropSize * 1, scale, cropSize, texture));
                break;
            case 'B':
                container->push_back(new Tile(cropSize * 10, cropSize * 1, scale, cropSize, texture));
                break;
            case 'C':
                container->push_back(new Tile(cropSize * 11, cropSize * 1, scale, cropSize, texture));
                break;
            case 'D':
                container->push_back(new Tile(cropSize * 12, cropSize * 1, scale, cropSize, texture));
                break;
            case 'E':
                container->push_back(new Tile(cropSize * 13, cropSize * 1, scale, cropSize, texture));
                break;
            case 'F':
                container->push_back(new Tile(cropSize * 14, cropSize * 1, scale, cropSize, texture));
                break;
                //AGUA
            case 'G':
                container->push_back(new Tile(cropSize * 15, cropSize * 1, scale, cropSize, texture));
                break;
            case 'H':
                container->push_back(new Tile(cropSize * 16, cropSize * 1, scale, cropSize, texture));
                break;
            case 'I':
                container->push_back(new Tile(cropSize * 17, cropSize * 1, scale, cropSize, texture));
                break;
            //SEGUNDA FILA
            //ARENA
            case 'J':
                container->push_back(new Tile(cropSize * 9, cropSize * 2, scale, cropSize, texture));
                break;
            case 'K':
                container->push_back(new Tile(cropSize * 10, cropSize * 2, scale, cropSize, texture));
                break;
            case 'L':
                container->push_back(new Tile(cropSize * 11, cropSize * 2, scale, cropSize, texture));
                break;
            case 'M':
                container->push_back(new Tile(cropSize * 12, cropSize * 2, scale, cropSize, texture));
                break;
            case 'N':
                container->push_back(new Tile(cropSize * 13, cropSize * 2, scale, cropSize, texture));
                break;
            case 'R':
                container->push_back(new Tile(cropSize * 14, cropSize * 2, scale, cropSize, texture));
                break;
                //AGUA
            case 'O':
                container->push_back(new Tile(cropSize * 15, cropSize * 2, scale, cropSize, texture));
                break;
            case 'P':
                container->push_back(new Tile(cropSize * 16, cropSize * 2, scale, cropSize, texture));
                break;
            case 'Q':
                container->push_back(new Tile(cropSize * 17, cropSize * 2, scale, cropSize, texture));
                break;
            case 'S':
                container->push_back(new Tile(cropSize * 18, cropSize * 2, scale, cropSize, texture));
                break;
            //ESCENARIO CALABOZO Y LAVA
            //CALABOZO PISO FILA 1
            case 'n':
                container->push_back(new Tile(cropSize * 20, cropSize * 1, scale, cropSize, texture));
                break;
            case 'o':
                container->push_back(new Tile(cropSize * 21, cropSize * 1, scale, cropSize, texture));
                break;
            case 'p':
                container->push_back(new Tile(cropSize * 22, cropSize * 1, scale, cropSize, texture));
                break;
            case 'q':
                container->push_back(new Tile(cropSize * 23, cropSize * 1, scale, cropSize, texture));
                break;
            case 'r':
                container->push_back(new Tile(cropSize * 24, cropSize * 1, scale, cropSize, texture));
                break;
            case 's':
                container->push_back(new Tile(cropSize * 25, cropSize * 1, scale, cropSize, texture));
                break;
                //CALABOZO PISO FILA 2
            case 't':
                container->push_back(new Tile(cropSize * 20, cropSize * 2, scale, cropSize, texture));
                break;
            case 'u':
                container->push_back(new Tile(cropSize * 21, cropSize * 2, scale, cropSize, texture));
                break;
            case 'v':
                container->push_back(new Tile(cropSize * 22, cropSize * 2, scale, cropSize, texture));
                break;
            case 'W':
                container->push_back(new Tile(cropSize * 23, cropSize * 2, scale, cropSize, texture));
                break;
            case 'X':
                container->push_back(new Tile(cropSize * 24, cropSize * 2, scale, cropSize, texture));
                break;
                //CALABOZO PICOS
            case 'Y':
                container->push_back(new Tile(cropSize * 27, cropSize * 4, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
                //CALABOZO PAREDES FILA 3
            case 'w':
                container->push_back(new Tile(cropSize * 20, cropSize * 3, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case 'x':
                container->push_back(new Tile(cropSize * 21, cropSize * 3, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case 'y':
                container->push_back(new Tile(cropSize * 22, cropSize * 3, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case 'z':
                container->push_back(new Tile(cropSize * 23, cropSize * 3, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case 'T':
                container->push_back(new Tile(cropSize * 24, cropSize * 3, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case 'U':
                container->push_back(new Tile(cropSize * 25, cropSize * 3, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case 'V':
                container->push_back(new Tile(cropSize * 26, cropSize * 3, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
                //LIMITES
                //AIRE LIBRE
            case '-':
                container->push_back(new Tile(cropSize * 1, cropSize * 7, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
                //valla zacate
            case '$': //Esquina superior
                container->push_back(new Tile(cropSize * 1, cropSize * 8, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '#': 
                container->push_back(new Tile(cropSize * 1, cropSize * 9, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '%': //Esquina inferior
                container->push_back(new Tile(cropSize * 1, cropSize * 10, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '@': //Siguiente de la esquina superior
                container->push_back(new Tile(cropSize * 2, cropSize * 8, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '!': 
                container->push_back(new Tile(cropSize * 3, cropSize * 8, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '+': 
                container->push_back(new Tile(cropSize * 4, cropSize * 8, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '&': 
                container->push_back(new Tile(cropSize * 5, cropSize * 8, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case ')': 
                container->push_back(new Tile(cropSize * 2, cropSize * 10, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '(': 
                container->push_back(new Tile(cropSize * 3, cropSize * 10, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '=': 
                container->push_back(new Tile(cropSize * 4, cropSize * 10, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '_': 
                container->push_back(new Tile(cropSize * 5, cropSize * 10, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
                //Piso
            case '.':
                container->push_back(new Tile(cropSize * 2, cropSize * 7, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '*':
                container->push_back(new Tile(cropSize * 9, cropSize * 8, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
                //vallas tierrita
            case '/': //esquina superior izquierda
                container->push_back(new Tile(cropSize * 1, cropSize * 11, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '`': //valla superior 1
                container->push_back(new Tile(cropSize * 2, cropSize * 11, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '|': //valla superior 2
                container->push_back(new Tile(cropSize * 3, cropSize * 11, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case ',': //valla superior 3
                container->push_back(new Tile(cropSize * 4, cropSize * 11, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '>': //esquina superior derecha
                container->push_back(new Tile(cropSize * 5, cropSize * 11, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '<': //valla izquierda pa bajo
                container->push_back(new Tile(cropSize * 1, cropSize * 12, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '~': //valla derecha pa bajo
                container->push_back(new Tile(cropSize * 5, cropSize * 12, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break; 
            case '{': //esquina inferior izquierda
                container->push_back(new Tile(cropSize * 1, cropSize * 13, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '}': // valla inferior 1
                container->push_back(new Tile(cropSize * 2, cropSize * 13, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case '^': // valla inferior 2
                container->push_back(new Tile(cropSize * 3, cropSize * 13, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case ';': // valla inferior 3
                container->push_back(new Tile(cropSize * 4, cropSize * 13, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;
            case ':': //esquina inferior derecha
                container->push_back(new Tile(cropSize * 5, cropSize * 13, scale, cropSize, texture, world));
                container->back()->TurnPhysicsOn(cropSize * scale * j, cropSize * scale * i);
                break;

            default:
                break;
            }
            container->back()->Move(cropSize * scale * j, cropSize * scale * i);
            //container->back()->TurnPhysicsOn(cropSize * scale* j, cropSize * scale * i);
        }
    }
}

std::vector<Tile *> *Maze::GetContainer() const
{
    return container;
}

char **Maze::GetTiles() const
{
    return tiles;
}

Maze::~Maze()
{
}