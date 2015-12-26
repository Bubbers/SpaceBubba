//
// Created by simon on 2015-12-22.
//

#ifndef BUBBA_3D_JOYSTICKBUTTON_H
#define BUBBA_3D_JOYSTICKBUTTON_H

#include <ControlStatus.h>
#include <Button.h>

class JoystickButton : public Button{
public:
    JoystickButton(unsigned int key);
    JoystickButton(unsigned int pos, unsigned int neg);
    ControlStatus getStatus();

private:
    unsigned int joystickPos,joystickNeg;

};


#endif //BUBBA_3D_JOYSTICKBUTTON_H
