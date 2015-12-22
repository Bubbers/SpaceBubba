//
// Created by simon on 2015-12-20.
//

#include <Button.h>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <ControlStatus.h>

ControlStatus Button::getStatus(){
    ControlStatus buttonsPressed = ControlStatus();
    int value = 0;
    switch(device){
        case KEYBOARD:
            value = (int)sf::Keyboard::isKeyPressed(keyPos);
            if(dual)
                value -= sf::Keyboard::isKeyPressed(keyNeg);
            buttonsPressed.addButton(ControlStatus::KEYBOARD,value*100);
            return buttonsPressed;
        case MOUSE:
            value = sf::Mouse::isButtonPressed(mButtonPos);
            if(dual)
                value -= sf::Mouse::isButtonPressed(mButtonNeg);
            buttonsPressed.addButton(ControlStatus::KEYBOARD,value*100);
        case JOYSTICK_BUTTON:
            for(unsigned int i = 0; i < sf::Joystick::Count; i++)
                if(sf::Joystick::isConnected(i)) {
                    value = sf::Joystick::isButtonPressed(i,joystickPos);
                    if(dual)
                        value -= sf::Joystick::isButtonPressed(i,joystickNeg);
                    buttonsPressed.addButton(ControlStatus::activatorFromJoystickNumber(i), value*100);
                }
            return buttonsPressed;
        case JOYSTICK_AXIS:
            for(unsigned int i = 0; i < sf::Joystick::Count; i++)
                if(sf::Joystick::isConnected(i) && sf::Joystick::hasAxis(i,axis)) {
                    float val = sf::Joystick::getAxisPosition(i, axis);
                    if(!dual)
                        val = (val+100)/2; //transform [-100,100] to [0,100]
                    buttonsPressed.addButton(ControlStatus::activatorFromJoystickNumber(i),val);
                }
            return buttonsPressed;
    }
}

bool Button::isDual() {
    return dual;
}

Button::Button(Device device, bool dual) {
    this->device = device;
    this->dual = dual;
}

Button::Button(sf::Mouse::Button id) : Button(MOUSE,false) {
    this->mButtonPos = id;
}

Button::Button(sf::Mouse::Button pos, sf::Mouse::Button neg) : Button(MOUSE,true) {
    this->mButtonPos = pos;
    this->mButtonNeg = neg;
}

Button::Button(sf::Joystick::Axis axis, bool dual) : Button(JOYSTICK_AXIS, dual){
    this->axis = axis;
}

/**
 * Creates a button representing a joystick button. The boolean
 * true is thrown if the number isn't below sf::Joystick::ButtonCount
 */
Button::Button(unsigned int joystickButton) : Button(JOYSTICK_BUTTON,false){
    if(joystickButton >= sf::Joystick::ButtonCount)
        throw true;
    this->joystickPos = joystickButton;
}

Button::Button(unsigned int joystickNeg, unsigned int joystickPos) : Button(JOYSTICK_BUTTON,true) {
    if(joystickPos >= sf::Joystick::ButtonCount || joystickNeg >= sf::Joystick::ButtonCount)
        throw true;
    this->joystickPos = joystickNeg;
    this->joystickNeg = joystickPos;
}

Button::Button(sf::Keyboard::Key key) : Button(KEYBOARD, false){
    this->keyPos = key;
}

Button::Button(sf::Keyboard::Key keyPos, sf::Keyboard::Key keyNeg) : Button(KEYBOARD,true){
    this->keyNeg = keyNeg;
    this->keyPos = keyPos;
}

