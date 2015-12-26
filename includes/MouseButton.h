//
// Created by simon on 2015-12-22.
//

#ifndef BUBBA_3D_MOUSEBUTTON_H
#define BUBBA_3D_MOUSEBUTTON_H

#include <SFML/Window.hpp>
#include <ControlStatus.h>
#include <Button.h>

class MouseButton : public Button {
public:
    MouseButton(sf::Mouse::Button button);
    MouseButton(sf::Mouse::Button buttonPos, sf::Mouse::Button buttonNeg);
    ControlStatus getStatus();

private:
    sf::Mouse::Button mButtonPos, mButtonNeg;
};


#endif //BUBBA_3D_MOUSEBUTTON_H
