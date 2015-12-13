
#ifndef BUBBA_3D_TIMEDLIFE_H
#define BUBBA_3D_TIMEDLIFE_H


#include <IComponent.h>
#include <GameObject.h>

class TimedLife : public IComponent{
public:
    TimedLife(GameObject *gob, float life);
    void update(float dt);

private:
    float life;
    GameObject *gob;
};


#endif //BUBBA_3D_TIMEDLIFE_H
