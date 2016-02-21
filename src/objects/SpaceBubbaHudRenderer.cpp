//
// Created by simon on 2016-02-21.
//

#include "SpaceBubbaHudRenderer.h"
#include <GLSquare.h>

void SpaceBubbaHudRenderer::render() {
    float rot = (220.0f * *spaceShipSpeed + 70.0f)*(float)M_PI/180.0f;
    arrow->setRotation(rot);
    HudRenderer::render();
}

SpaceBubbaHudRenderer::SpaceBubbaHudRenderer(float *spaceShipSpeed)  : spaceShipSpeed(spaceShipSpeed), HudRenderer(){}

void SpaceBubbaHudRenderer::setLayout(Layout *layout) {
    HudRenderer::setLayout(layout);
    arrow = getSquareByID("speedArrow");
}