//
// Created by basse on 2015-12-12.
//

#ifndef BUBBA_3D_WEAPON_H
#define BUBBA_3D_WEAPON_H


#include "Projectile.h"
class Projectile;
class Weapon {

    public:
        virtual void fire() = 0;
    protected:
        static void destroyAfter(int seconds, Projectile* projectile);
};


#endif //BUBBA_3D_WEAPON_H
