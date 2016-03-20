//
// Created by simon on 2016-02-21.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAHUDRENDERER_H
#define SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAHUDRENDERER_H

#include <HudRenderer.h>
#include <State.h>

class IHudDrawable;
class Font;
class TextObject;

class SpaceBubbaHudRenderer : public HudRenderer {

public:

    virtual void update(float dt);
    SpaceBubbaHudRenderer(float* spaceShipSpeed, int* points, State* state);

private:
    float* spaceShipSpeed;
    State* state, prevState;
    IHudDrawable *arrow;
    TextObject *scoreObject;
    int prevScore = 0, *points;
    Font* ubuntu30, *ubuntu100;

    Layout* createPlayingLayout();
    Layout* createStartLayout();
    Layout* createCreditsLayout();
    Layout* createFailedLayout();
    Layout* createSuccessLayout();
    Layout* createMessageLayout(string message, Font* font);

};


#endif //SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAHUDRENDERER_H
