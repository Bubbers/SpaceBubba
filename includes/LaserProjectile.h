//
// Created by basse on 2015-12-12.
//

#ifndef BUBBA_3D_LASERPROJECTILE_H
#define BUBBA_3D_LASERPROJECTILE_H
#include "Projectile.h"

class LaserProjectile : public Projectile {

    private:

        int baseDamage;
        float velocity;

    public:
        ~LaserProjectile();
        LaserProjectile(int baseDamage);
        int calculateDamage();
};


#endif //BUBBA_3D_LASERPROJECTILE_H
