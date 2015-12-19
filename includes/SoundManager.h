//
// Created by johan on 2015-12-12.
//

#ifndef BUBBA_3D_SOUNDMANAGER_H
#define BUBBA_3D_SOUNDMANAGER_H

#include <SFML/Audio.hpp>

using namespace std;


class SoundManager {
public:

    SoundManager();
    ~SoundManager();



private:
    sf::Music ambient;

    sf::SoundBuffer buffer;
    sf::Sound sound;
};


#endif //BUBBA_3D_SOUNDMANAGER_H