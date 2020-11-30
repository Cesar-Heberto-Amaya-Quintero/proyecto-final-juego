#include "SFX.hh"


SFX::SFX()
{
    soundBuffer = new sf::SoundBuffer();
    sound = new sf::Sound();

    soundList = new const char*[2]
    {
        "assets/audio/meat.ogg" ,
        "assets/audio/perroLlorando.ogg"       
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