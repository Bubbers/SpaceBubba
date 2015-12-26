//
// Created by simon on 2015-12-22.
//

#include <MouseButton.h>
#include <Button.h>

MouseButton::MouseButton(sf::Mouse::Button button) : Button(false){
    this->mButtonPos = button;
}

MouseButton::MouseButton(sf::Mouse::Button pos, sf::Mouse::Button neg) : Button(true) {
    this->mButtonPos = pos;
    this->mButtonNeg = neg;
}

ControlStatus MouseButton::getStatus() {
    float value = sf::Mouse::isButtonPressed(mButtonPos) ? 1.0f : 0.0f;
    if(dual)
        value -= sf::Mouse::isButtonPressed(mButtonPos) ? 1.0f : 0.0f;
    return ControlStatus(ControlStatus::KEYBOARD, value*100.0f);
}
