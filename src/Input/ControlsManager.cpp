//
// Created by simon on 2015-12-20.
//

#include <ControlsManager.h>
#include <map>
#include <vector>
#include <ControlStatus.h>

/**
 * Gets the singleton instance of the controls manager
 */
ControlsManager* ControlsManager::getInstance() {

    static ControlsManager instance; // Guaranteed to be destroyed.
    // Instantiated on first use.
    return &instance;

}

ControlsManager::ControlsManager() { }
ControlsManager::~ControlsManager() {
    for(auto i = functions.begin(); i!= functions.end() ; i++)
        for(auto j = i->second.begin() ; j != i->second.end() ; j++)
            delete (*j); //Delete the buttons
}

void ControlsManager::addBinding(int function, Button* button) {
    addBindings(function,{button});
}

void ControlsManager::addBindings(int function, std::initializer_list<Button*> buttons) {
    functionCollection::iterator elem = functions.find(function);
    std::vector<Button*> list;
    if(elem != functions.end())
        list = elem->second;
    for(auto it = buttons.begin() ; it != buttons.end(); it++)
        list.insert(list.end(),*it);
    functions.insert(std::pair<int,std::vector<Button*>>(function,list));
}

ControlStatus ControlsManager::getStatus(int function) {
    std::vector<Button*> buttons;
    functionCollection::iterator elem = functions.find(function);
    if(elem == functions.end())
        return  ControlStatus();
    buttons = elem->second;
    ControlStatus status = ControlStatus();
    for(auto it = buttons.begin(); it != buttons.end() ; it++)
        status.merge((*it)->getStatus());
    return status;
}
