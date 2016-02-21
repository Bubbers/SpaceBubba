//
// Created by simon on 2016-02-21.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAHUDRENDERER_H
#define SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAHUDRENDERER_H

#include <HudRenderer.h>

class GLSquare;

class SpaceBubbaHudRenderer : public HudRenderer {

public:

    virtual void render();
    SpaceBubbaHudRenderer(float* spaceShipSpeed);
    virtual void setLayout(Layout* layout);

private:
    float* spaceShipSpeed;
    GLSquare* arrow;

};


#endif //SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAHUDRENDERER_H
