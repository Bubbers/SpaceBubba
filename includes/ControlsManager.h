// Created by simon on 2015-12-20.
//

#ifndef BUBBA_3D_CONTROLSMANAGER_H
#define BUBBA_3D_CONTROLSMANAGER_H

#include <Button.h>

typedef std::map<int,std::vector<Button>> functionCollection ;

class ControlsManager {
public:
    void addBinding(int function, Button button);
    void addBindings(int function, std::initializer_list<Button> buttons);
    ControlStatus getStatus(int function);
    static ControlsManager* getInstance();

private:
    ControlsManager();
    functionCollection functions;
};


#endif //BUBBA_3D_CONTROLSMANAGER_H
