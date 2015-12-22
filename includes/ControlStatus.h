//
// Created by simon on 2015-12-20.
//

#ifndef BUBBA_3D_BUTTONSTATUS_H
#define BUBBA_3D_BUTTONSTATUS_H

#include <map>

class ControlStatus {
public:
    enum Activator  {NONE = 1024,KEYBOARD = 256, MOUSE = 512, JOYSTICK_0 = 1, JOYSTICK_1 = 2, JOYSTICK_2 = 4,
                    JOYSTICK_3 = 8, JOYSTICK_4 = 16, JOYSTICK_5 = 32, JOYSTICK_6 = 64, JOYSTICK_7 = 128};

    static Activator activatorFromJoystickNumber(int n);

    /**
     * Returns 100 or zero for binary buttons and a float [-100,100] for analog.
     * Returns the maximum value from any activator.
     */
    float getValue();

    /**
     * Gets the value from the specified activator.
     * Returns 100 or zero for binary buttons and a float [-100,100] for analog.
     */
    float getValue(Activator activator);

    /*
     * Alias for:
     * return getValue() > 20;
     */
    bool isActive();

    /**
     * Alias for:
     * return getValue(Activator) > 20;
     */
    bool isActive(Activator activator);
    ControlStatus(Activator activator, float value);
    ControlStatus();
    void addButton(Activator activator, float value);
    void merge(ControlStatus cs);

private:
    std::map<Activator,float> buttons;

};


#endif //BUBBA_3D_BUTTONSTATUS_H
