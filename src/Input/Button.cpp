//
// Created by simon on 2015-12-20.
//

#include <Button.h>
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <ControlStatus.h>

bool Button::isDual() {
    return dual;
}

Button::Button(bool dual) {
    this->dual = dual;
}