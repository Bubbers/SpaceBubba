//
// Created by simon on 2015-12-22.
//

#ifndef BUBBA_3D_KEYBOARDBUTTON_H
#define BUBBA_3D_KEYBOARDBUTTON_H

#include <SFML/Window.hpp>
#include <ControlStatus.h>
#include <Button.h>

class KeyboardButton : public Button {
public:
    KeyboardButton(sf::Keyboard::Key key);
    KeyboardButton(sf::Keyboard::Key keyPos, sf::Keyboard::Key keyNeg);
    ControlStatus getStatus();

private:
    sf::Keyboard::Key keyPos,keyNeg;

};


#endif //BUBBA_3D_KEYBOARDBUTTON_H
