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

    virtual ControlStatus getStatus() = 0;
    bool isDual();

protected:
    Button(bool dual);
    bool dual;

};


#endif //BUBBA_3D_BUTTON_H
