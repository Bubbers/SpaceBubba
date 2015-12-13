#include "Scene.h"


Scene::Scene()
{
}


Scene::~Scene()
{
}


void Scene::update(float dt) {
    removeDirty(&shadowCasters);
    removeDirty(&transparentObjects);

    for(auto &object : shadowCasters ) {
        object->update(dt);
    }

    for(auto &object : transparentObjects ) {
        object->update(dt);
    }
}

void Scene::removeDirty(std::vector<GameObject*> *v) {
    for(auto i = v->begin(); v->end() != i; i++)
    {
        if((*i)->isDirty())
        {
            i = v->erase(i);
        }
    }
}