//
// Created by basse on 2015-12-12.
//



#ifndef BUBBA_3D_SPACESHIP_H
#define BUBBA_3D_SPACESHIP_H
#include "Weapon.h"
#include "Projectile.h"

class Weapon;
class Projectile;
class SpaceShip {
private:
    int maxHealth;
    int currentHealth;
    Weapon* weapon;

public:

    ~SpaceShip();
    SpaceShip();

    bool dead();
    bool takeDamage(Projectile* projectile);
    void shoot();

};
#endif //BUBBA_3D_SPACESHIP_H
