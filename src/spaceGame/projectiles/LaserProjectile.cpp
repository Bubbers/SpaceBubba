//
// Created by basse on 2015-12-12.
//

#include "../../../includes/LaserProjectile.h"

LaserProjectile::~LaserProjectile() {

}

LaserProjectile::LaserProjectile(int damage) {
    baseDamage = damage;
}

int LaserProjectile::calculateDamage() {
    return baseDamage;
}
