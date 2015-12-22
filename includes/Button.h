//
// Created by simon on 2015-12-20.
//

#ifndef BUBBA_3D_BUTTON_H
#define BUBBA_3D_BUTTON_H

#include <ControlStatus.h>
#include <SFML/Window.hpp>

class Button {
public:
    enum MOUSE : int {LEFT_MOUSE = 0,RIGHT_MOUSE = 1,MIDDLE_MOUSE = 2,EXTRA1 = 3,EXTRA2 = 4};

    ControlStatus getStatus();
    Button(sf::Mouse::Button id);
    Button(sf::Mouse::Button pos,sf::Mouse::Button neg);
    Button(unsigned int joystickButton);
    Button(unsigned int joystickNeg, unsigned int joystickPos);
    Button(sf::Joystick::Axis axis, bool dual);
    Button(sf::Keyboard::Key id);
    Button(sf::Keyboard::Key keyNeg, sf::Keyboard::Key keyPos);
    bool isDual();

private:
    enum Device {KEYBOARD = 0, MOUSE = 1, JOYSTICK_BUTTON = 2, JOYSTICK_AXIS = 3};
    Button(Device device, bool dual);
    Device device;
    bool dual;
    unsigned int joystickPos, joystickNeg;
    sf::Mouse::Button mButtonPos, mButtonNeg;
    sf::Keyboard::Key keyPos, keyNeg;
    sf::Joystick::Axis axis;

};


#endif //BUBBA_3D_BUTTON_H
