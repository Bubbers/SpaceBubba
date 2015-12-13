//
// Created by simon on 2015-12-12.
//

#ifndef BUBBA_3D_SPACESHIPCOMPONENT_H
#define BUBBA_3D_SPACESHIPCOMPONENT_H




class SpaceShipComponent : public MoveComponent{
public:

    SpaceShipComponent(struct HudRenderer::HudConfig* hudConf, float* cameraThetaLocation, GameObject* ship);

    void update(float dt) ;
    float3 getFrontDir();

private:
    struct HudRenderer::HudConfig* hudConf;

    float* cameraThetaLocation;
    void checkKeyPresses();

    float rotationSpeed = (float) (2 * M_PI / 180);
    float3 frontDir = make_vector(0.0f, 0.0f, 1.0f);


};


#endif //BUBBA_3D_SPACESHIPCOMPONENT_H
