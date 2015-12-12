//
// Created by basse on 2015-12-12.
//

#include "../../../includes/Weapon.h"
#include "../../../includes/Projectile.h"
#include <thread>

void Weapon::destroyAfter(int time, Projectile* projectile) {
    printf("starting delete");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    delete projectile;
    printf("should be dead");
}