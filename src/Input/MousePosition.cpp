//
// Created by simon on 2015-12-29.
//

#include <MousePosition.h>
#include <SFML/Window.hpp>
#include <GL/freeglut_std.h>

MousePosition::MousePosition() {
    update();
}

void MousePosition::update(){
    sf::Vector2i pos = sf::Mouse::getPosition();
    x = pos.x;
    y = pos.y;
}

int MousePosition::getRawX() {
    return x;
}

int MousePosition::getRawY() {
    return y;
}

int MousePosition::getWindowX() {
    return x-glutGet(GLUT_WINDOW_X);
}

int MousePosition::getWindowY() {
    return y-glutGet(GLUT_WINDOW_Y);
}

int MousePosition::getXMidOffset() {
    return getWindowX() - glutGet(GLUT_WINDOW_WIDTH)/2;
}

int MousePosition::getYMidOffset() {
    return getWindowY() - glutGet(GLUT_WINDOW_HEIGHT)/2;
}