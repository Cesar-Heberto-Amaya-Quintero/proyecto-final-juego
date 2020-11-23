#include "SFX.hh"


SFX::SFX()
{
    soundBuffer = new sf::SoundBuffer();
    sound = new sf::Sound();

    soundList = new const char*[1]
    {
        "assets/audio/musicaFondoAfuera.ogg"        
    };
}

SFX::~SFX()
{
}

void SFX::PlaySFX(int index)
{
    soundBuffer->loadFromFile(soundList[index]);
    sound->setBuffer(*soundBuffer);
    sound->setVolume(30.f);
    sound->play();
}