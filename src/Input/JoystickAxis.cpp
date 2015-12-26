//
// Created by simon on 2015-12-22.
//

#include <JoystickAxis.h>
#include <SFML/Window.hpp>
#include <Button.h>

JoystickAxis::JoystickAxis(sf::Joystick::Axis axis, bool dual) : Button(dual){
    this->axis = axis;
}

ControlStatus JoystickAxis::getStatus() {
    ControlStatus buttonsPressed = ControlStatus();
    for(unsigned int i = 0; i < sf::Joystick::Count; i++)
        if(sf::Joystick::isConnected(i) && sf::Joystick::hasAxis(i,axis)) {
            float val = sf::Joystick::getAxisPosition(i, axis);
            if(!dual)
                val = (val+100)/2; //transform [-100,100] to [0,100]
            buttonsPressed.addButton(ControlStatus::activatorFromJoystickNumber(i),val);
        }
    return buttonsPressed;
}