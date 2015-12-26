//
// Created by simon on 2015-12-22.
//

#include <JoystickButton.h>
#include <SFML/Window/Joystick.hpp>
#include <Button.h>

/**
 * Creates a button representing a joystick button. The boolean
 * true is thrown if the number isn't below sf::Joystick::ButtonCount
 */
JoystickButton::JoystickButton(unsigned int joystickButton) : Button(false){
    if(joystickButton >= sf::Joystick::ButtonCount)
        throw true;
    this->joystickPos = joystickButton;
}

JoystickButton::JoystickButton(unsigned int joystickNeg, unsigned int joystickPos) : Button(true) {
    if(joystickPos >= sf::Joystick::ButtonCount || joystickNeg >= sf::Joystick::ButtonCount)
        throw true;
    this->joystickPos = joystickNeg;
    this->joystickNeg = joystickPos;
}


ControlStatus JoystickButton::getStatus() {
    float value;
    ControlStatus cs = ControlStatus();
    for(unsigned int i = 0; i < sf::Joystick::Count; i++)
        if(sf::Joystick::isConnected(i)) {
            value = sf::Joystick::isButtonPressed(i,joystickPos) ? 1 : 0;
            if(dual)
                value -= sf::Joystick::isButtonPressed(i,joystickNeg) ? 1 : 0;
            cs.addButton(ControlStatus::activatorFromJoystickNumber(i), value*100);
        }
    return cs;
}