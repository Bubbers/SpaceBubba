//
// Created by basse on 2015-12-12.
//

#ifndef BUBBA_3D_LASERCANNON_H
#define BUBBA_3D_LASERCANNON_H


#include "Weapon.h"
#include "Projectile.h"

class LaserCannon : public Weapon {
    public:
        ~LaserCannon();
        LaserCannon();
        static int baseDamage;
        void fire();

private:
    void destroyAfter(int time, Projectile* projectile);
};


#endif //BUBBA_3D_LASERCANNON_H
