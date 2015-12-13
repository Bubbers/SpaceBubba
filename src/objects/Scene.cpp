#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}


void Scene::update(float dt, std::vector<GameObject*> *toDelete) {
    removeDirty(&shadowCasters, toDelete);
    removeDirty(&transparentObjects, toDelete);

    for(auto &object : shadowCasters ) {
        object->update(dt);
    }

    for(auto &object : transparentObjects ) {
        object->update(dt);
    }
}

void Scene::removeDirty(std::vector<GameObject*> *v, std::vector<GameObject*> *toDelete) {
    for(auto i = v->begin(); i < v->end(); i++)
    {
        if((*i)->isDirty())
        {
            toDelete->push_back(*i);
            i = v->erase(i);
            /*if(i == v->end()) {
                return;
            }*/
        }
    }
}