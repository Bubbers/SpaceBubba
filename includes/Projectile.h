//
// Created by basse on 2015-12-12.
//

#ifndef BUBBA_3D_PROJECTILE_H
#define BUBBA_3D_PROJECTILE_H


#include "SpaceShip.h"
class Spaceship;
class Projectile {

    public:
        void timeToLive (int time);
        virtual int calculateDamage() = 0;

};


#endif //BUBBA_3D_PROJECTILE_H
