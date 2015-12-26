//
// Created by simon on 2015-12-22.
//

#include <KeyboardButton.h>
#include <Button.h>

KeyboardButton::KeyboardButton(sf::Keyboard::Key key) : Button(false){
    this->keyPos = key;
}

KeyboardButton::KeyboardButton(sf::Keyboard::Key keyPos, sf::Keyboard::Key keyNeg) : Button(true){
    this->keyNeg = keyNeg;
    this->keyPos = keyPos;
}

ControlStatus KeyboardButton::getStatus() {
    float value = sf::Keyboard::isKeyPressed(keyPos) ? 1.0f : 0.0f;
    if(dual)
        value -= sf::Keyboard::isKeyPressed(keyNeg) ? 1.0f : 0.0f;
    return ControlStatus(ControlStatus::KEYBOARD,value*100.0f);
}