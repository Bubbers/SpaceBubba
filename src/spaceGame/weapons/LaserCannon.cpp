//
// Created by basse on 2015-12-12.
//

#include "../../../includes/LaserCannon.h"
#include "../../../includes/LaserProjectile.h"
#include <thread>
int LaserCannon::baseDamage = 10;

void LaserCannon::fire() {
    LaserProjectile* projectile = new LaserProjectile(baseDamage);
    std::thread timeToLive(Weapon::destroyAfter,2, projectile);
    timeToLive.join();
}

LaserCannon::LaserCannon() {

}

LaserCannon::~LaserCannon() {

}
