//
// Created by simon on 2015-12-12.
//

#ifndef BUBBA_3D_SPACESHIPCOMPONENT_H
#define BUBBA_3D_SPACESHIPCOMPONENT_H


class SpaceShipComponent : public MoveComponent{
public:

    SpaceShipComponent(struct HudRenderer::HudConfig* hudConf, float* cameraThetaLocation, GameObject* ship);

    void update(float dt) ;

private:
    struct HudRenderer::HudConfig* hudConf;

    float* cameraThetaLocation;
    void checkKeyPresses();

    float rotationSpeed = (float) (2 * M_PI / 180);

};


#endif //BUBBA_3D_SPACESHIPCOMPONENT_H
