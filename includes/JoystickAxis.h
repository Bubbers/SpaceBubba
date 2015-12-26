//
// Created by simon on 2015-12-22.
//

#ifndef BUBBA_3D_JOYSTICKAXIS_H
#define BUBBA_3D_JOYSTICKAXIS_H

#include <SFML/Window.hpp>
#include <ControlStatus.h>
#include <Button.h>

class JoystickAxis : public Button {
public:
    JoystickAxis(sf::Joystick::Axis axis, bool dual);
    ControlStatus getStatus();

private:
    sf::Joystick::Axis axis;

};


#endif //BUBBA_3D_JOYSTICKAXIS_H
