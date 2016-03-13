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

void SpaceBubbaHudRenderer::render() {
    if (!(*state != Playing && *state == prevState)) {

        if (*state == Playing) {
            if (prevState != Playing) {
                setLayout(createPlayingLayout());
                arrow = getSquareByID("speedArrow");
            }
            if (*points != prevScore) {
                TextLayout *score = (TextLayout *) getLayoutById("num");
                score->setText(to_string(*points));
                updateLayout();
                arrow = getSquareByID("speedArrow");
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
    HudRenderer::render();


}

Layout* SpaceBubbaHudRenderer::createMessageLayout(string message, Font* font) {

    PositioningLayout* root = new PositioningLayout(Dimension::fromPercentage(100),Dimension::fromPercentage(100));
    root->addChild((new TextLayout(message,font,Dimension::fromPercentage(60), Dimension::fromPercentage(50)))
                           ->setBackground(new HUDGraphic(HUDGraphic::Color(new string("#0f3b0c"),0.522f)))
            ,Dimension::fromPercentage(20),Dimension::fromPercentage(25));
    setLayout(root);
    return root;

}

SpaceBubbaHudRenderer::SpaceBubbaHudRenderer(float *spaceShipSpeed, int* points, State* state)
        : spaceShipSpeed(spaceShipSpeed), points(points), state(state), prevState(Won), HudRenderer(){}

Layout* SpaceBubbaHudRenderer::createStartLayout() {

    Font* font = FontManager().loadAndFetchFont("../fonts/Ubuntu-M.ttf",30);
    return createMessageLayout(hudStringWelcome,font);

}

Layout* SpaceBubbaHudRenderer::createCreditsLayout() {
    Font* font = FontManager().loadAndFetchFont("../fonts/Ubuntu-M.ttf",30);

    return createMessageLayout(hudStringCredits,font);
}

Layout* SpaceBubbaHudRenderer::createFailedLayout() {

    Font* font = FontManager().loadAndFetchFont("../fonts/Ubuntu-M.ttf",100);
    return createMessageLayout(hudStringFailure,font);
}

Layout* SpaceBubbaHudRenderer::createSuccessLayout() {

    Font* font = FontManager().loadAndFetchFont("../fonts/Ubuntu-M.ttf",100);
    return createMessageLayout(hudStringSuccess,font);
}

Layout* SpaceBubbaHudRenderer::createPlayingLayout(){
    Texture* meter = ResourceManager::loadAndFetchTexture("../scenes/HUD/meter2.0.png");
    Texture* arrow = ResourceManager::loadAndFetchTexture("../scenes/HUD/arrow.png");

    Font* font = FontManager().loadAndFetchFont("../fonts/Ubuntu-M.ttf",30);

    Layout* rootLayout = new ListLayout(ListLayout::VERTICAL,Dimension::fromPercentage(100),Dimension::fromPercentage(100));
    rootLayout->addChild(new ListLayout(ListLayout::VERTICAL,Dimension::fill(),Dimension::fill()));
    Layout* bottomBar = new ListLayout(ListLayout::HORIZONTAL,Dimension::fill(),Dimension::wrap());
    PositioningLayout* pointContainer = new PositioningLayout(Dimension::fill(),Dimension::fromPixels(200));
    pointContainer->addChild((new TextLayout("0",font,Dimension::fromPixels(100),Dimension::fromPixels(30)))->setId("num"),
                             Dimension::fromPixels(50),Dimension::fromPixels(90));
    bottomBar->addChild(pointContainer);

    PositioningLayout* meterL = new PositioningLayout(Dimension::fromPixels(200),Dimension::fromPixels(200));
    meterL->setBackground(new HUDGraphic(meter));
    PositioningLayout* arrowL = new PositioningLayout(Dimension::fromPixels(20),Dimension::fromPixels(120));
    arrowL->setBackground((new HUDGraphic(arrow))
                                  ->setCenterOffset(Dimension::fromPercentage(0),Dimension::fromPercentage(-33.0f)));
    arrowL->setId("speedArrow");
    meterL->addChild(arrowL,Dimension::fromPixels(90),Dimension::fromPixels(80));

    bottomBar->addChild(meterL);
    rootLayout->addChild(bottomBar);
    return rootLayout;
}