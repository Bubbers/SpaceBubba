//
// Created by johan on 2015-11-29.
//

#ifndef BUBBA_3D_COMPONENT_H
#define BUBBA_3D_COMPONENT_H



class IComponent {
public:
    virtual void update(float dt) = 0;

    virtual void beforeCollision() {};
    virtual void duringCollision() {};
    virtual void afterCollision() {};
    virtual void onDeath() {};
private:
};

#endif //BUBBA_3D_COMPONENT_H
