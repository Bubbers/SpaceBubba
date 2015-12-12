
#include "TimedLife.h"


TimedLife::TimedLife(GameObject *gob, float life) {
    this->gob = gob;
    this->life = life;
}

void TimedLife::update(float dt) {
    life -= dt;
    if (life <= 0.0f) {
        gob->makeDirty();
    }
}
