//
// Created by simon on 2016-02-21.
//

#include "SpaceBubbaHudRenderer.h"
#include <IHudDrawable.h>
#include <TextLayout.h>
#include <State.h>
#include <ResourceManager.h>
#include <FontManager.h>
#include <ListLayout.h>
#include <PositioningLayout.h>
#include <HUDGraphic.h>
#include <HudStrings.h>
#include <SFML/Window/Keyboard.hpp>
#include <TextObject.h>

void SpaceBubbaHudRenderer::update(float dt) {

    if (*state == Playing || *state != prevState) {

        if (*state == Playing) {
            if (prevState != Playing) {

                setLayout(createPlayingLayout());
                arrow = getHudDrawableById("speedArrow");
                scoreObject = (TextObject*)getHudDrawableById("num");

            }else if (*points != prevScore) {

                scoreObject->setText(to_string(*points));
                prevScore = *points;

            }
            float rot = (220.0f * *spaceShipSpeed + 70.0f) * (float) M_PI / 180.0f;
            arrow->setRotation(rot);
        } else if (*state == Start) {
            setLayout(createStartLayout());
        } else if (*state == Credits){
            setLayout(createCreditsLayout());
        } else if (*state == Died) {
            setLayout(createFailedLayout());
        } else if (*state == Won) {
            setLayout(createSuccessLayout());
        }
        prevState = *state;
    }

}

SpaceBubbaHudRenderer::SpaceBubbaHudRenderer(float *spaceShipSpeed, int* points, State* state)
        : spaceShipSpeed(spaceShipSpeed), points(points), state(state), prevState(Won), HudRenderer(){

    FontManager* fm = FontManager::getInstance();
    ubuntu30 = fm->loadAndFetchFont("../fonts/Ubuntu-M.ttf",30);
    ubuntu100 = fm->loadAndFetchFont("../fonts/Ubuntu-M.ttf",100);

    setLayout(createStartLayout());

}

Layout* SpaceBubbaHudRenderer::createMessageLayout(string message, Font* font, bool bigHeader) {

    PositioningLayout* root = new PositioningLayout(Dimension::fromPercentage(100),Dimension::fromPercentage(100));
    HUDGraphic* boxGraphic = new HUDGraphic(HUDGraphic::Color(string("#0f3b0c"),0.522f));
    boxGraphic->setRoundedCorners(40)->setBorder(bigHeader ? 43 : 3,3,3,3,HUDGraphic::Color(string("#0f3b0c")));
    root->addChild((new TextLayout(message,font,Dimension::fromPercentage(60), Dimension::fromPercentage(50)))
                           ->setPadding(0,5)
                           ->setBackground(boxGraphic),Dimension::fromPercentage(20),Dimension::fromPercentage(25));
    setLayout(root);
    return root;

}

Layout* SpaceBubbaHudRenderer::createStartLayout() {

    return createMessageLayout(hudStringWelcome,ubuntu30,true);

}

Layout* SpaceBubbaHudRenderer::createCreditsLayout() {

    return createMessageLayout(hudStringCredits,ubuntu30, true);
}

Layout* SpaceBubbaHudRenderer::createFailedLayout() {

    return createMessageLayout(hudStringFailure,ubuntu100, false);
}

Layout* SpaceBubbaHudRenderer::createSuccessLayout() {

    return createMessageLayout(hudStringSuccess,ubuntu100, false);
}

Layout* SpaceBubbaHudRenderer::createPlayingLayout(){
    Texture* meter = ResourceManager::loadAndFetchTexture("../scenes/HUD/meter2.0.png");
    Texture* arrow = ResourceManager::loadAndFetchTexture("../scenes/HUD/arrow.png");

    Layout* rootLayout = new ListLayout(ListLayout::VERTICAL,Dimension::fromPercentage(100),Dimension::fromPercentage(100));
    rootLayout->addChild(new ListLayout(ListLayout::VERTICAL,Dimension::fill(),Dimension::fill()));
    Layout* bottomBar = new ListLayout(ListLayout::HORIZONTAL,Dimension::fill(),Dimension::wrap());
    PositioningLayout* pointContainer = new PositioningLayout(Dimension::fill(),Dimension::fromPixels(200));
    pointContainer->addChild((new TextLayout("0",ubuntu30,Dimension::fromPixels(100),Dimension::fromPixels(30)))->setTextId("num"),
                             Dimension::fromPixels(50),Dimension::fromPixels(90));
    bottomBar->addChild(pointContainer);

    PositioningLayout* meterL = new PositioningLayout(Dimension::fromPixels(200),Dimension::fromPixels(200));
    meterL->setBackground(new HUDGraphic(meter));
    PositioningLayout* arrowL = new PositioningLayout(Dimension::fromPixels(20),Dimension::fromPixels(120));
    arrowL->setBackground((new HUDGraphic(arrow))
                                  ->setCenterOffset(Dimension::fromPercentage(0),Dimension::fromPercentage(-33.0f)));
    arrowL->setLayoutId("speedArrow");
    meterL->addChild(arrowL,Dimension::fromPixels(90),Dimension::fromPixels(80));

    bottomBar->addChild(meterL);
    rootLayout->addChild(bottomBar);
    return rootLayout;
}