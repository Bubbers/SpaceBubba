//
// Created by simon on 2016-02-21.
//

#include "SpaceBubbaHudRenderer.h"
#include <IHudDrawable.h>
#include <TextLayout.h>

void SpaceBubbaHudRenderer::render() {
    if(*points != prevScore){
        TextLayout* score = (TextLayout*)getLayoutById("num");
        score->setText(to_string(*points));
        updateLayout();
        arrow = getSquareByID("speedArrow");
        prevScore = *points;
    }
    float rot = (220.0f * *spaceShipSpeed + 70.0f)*(float)M_PI/180.0f;
    arrow->setRotation(rot);
    HudRenderer::render();
}

SpaceBubbaHudRenderer::SpaceBubbaHudRenderer(float *spaceShipSpeed, int* points)
        : spaceShipSpeed(spaceShipSpeed), points(points), HudRenderer(){}

void SpaceBubbaHudRenderer::setLayout(Layout *layout) {
    HudRenderer::setLayout(layout);
    arrow = getSquareByID("speedArrow");
}