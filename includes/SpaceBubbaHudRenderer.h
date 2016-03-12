//
// Created by simon on 2016-02-21.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAHUDRENDERER_H
#define SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAHUDRENDERER_H

#include <HudRenderer.h>

class IHudDrawable;

class SpaceBubbaHudRenderer : public HudRenderer {

public:

    virtual void render();
    SpaceBubbaHudRenderer(float* spaceShipSpeed, int* points);
    virtual void setLayout(Layout* layout);

private:
    float* spaceShipSpeed;
    IHudDrawable* arrow;
    int prevScore = 0, *points;

};


#endif //SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAHUDRENDERER_H
