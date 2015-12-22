//
// Created by simon on 2015-12-20.
//

#include "ControlStatus.h"
#include <map>

ControlStatus::ControlStatus() {
}

ControlStatus::ControlStatus(Activator activator, float value) {
    this->addButton(activator,value);
}

void ControlStatus::addButton(Activator activator, float value) {
    buttons.insert(std::pair<Activator,int>(activator,value));
}

void ControlStatus::merge(ControlStatus cs) {
    for(auto it = cs.buttons.begin(); it != cs.buttons.end(); it++)
        buttons.insert(std::pair<Activator,int>(it->first,it->second));
}

ControlStatus::Activator ControlStatus::activatorFromJoystickNumber(int n) {
    if(n >= 8)
        throw "Argument must be less than 8";
    const Activator map[8] = {JOYSTICK_0,JOYSTICK_1,JOYSTICK_2,JOYSTICK_3,JOYSTICK_4,JOYSTICK_5,JOYSTICK_6,JOYSTICK_7};
    return map[n];
}

float ControlStatus::getValue() {
    float maxValue = 0;
    for(auto it = buttons.begin() ; it != buttons.end() ; it++)
        if(it->second > 0 && it->second > maxValue || it->second < 0 && it->second < -maxValue)
            maxValue = it->second;
    return maxValue;
}

float ControlStatus::getValue(Activator activator) {
    std::map<Activator ,float>::iterator elem = buttons.find(activator);
    return elem == buttons.end() ? 0 : elem->second;
}

bool ControlStatus::isActive(){
    float val = getValue();
    return val > 20.0f || val < -20.0f;
}

bool ControlStatus::isActive(Activator activator) {
    float val = getValue();
    return val > 20 || val < -20.0f;
}
