
#include <Logger.h>
#include "GameObject.h"
#include "float3x3.h"
#include "Collider.h"

#define NORMAL_TEXTURE_LOCATION 3
#define DIFFUSE_TEXTURE_LOCATION 0

int GameObject::uniqueId = 0;

GameObject::GameObject() {
    m_modelMatrix = make_identity<float4x4>();
    id = ++uniqueId;
}

GameObject::GameObject(Mesh *mesh, GameObjectType type) {
    this->type = type;
    this->mesh = mesh;
    this->m_modelMatrix = make_identity<float4x4>();
    this->shininess = 0.0f;
    this->id = ++uniqueId;

    this->octree = createOctree(this->mesh);
};

GameObject::~GameObject() {
    mesh = nullptr;

}

Octree* GameObject::createOctree(Mesh* mesh) {
    AABB* aabb = this->mesh->getAABB();
    float3 halfVector = (aabb->maxV - aabb->minV) / 2;
    float3 origin = aabb->maxV - halfVector;
    Octree* octree = new Octree(origin, halfVector, 0);

    std::vector<Triangle *> triangles = mesh->getTriangles();
    octree->insertAll(triangles);

    return octree;
}


void GameObject::makeDirty() {
    for(auto &component : components)
        component->onDeath();
    dirty = true;
}

bool GameObject::isDirty() {
    return dirty;
}

void GameObject::move(float4x4 model_matrix) {
    m_modelMatrix = model_matrix;
}

void GameObject::update(float4x4 update_matrix) {
    m_modelMatrix = m_modelMatrix * update_matrix;
}

void GameObject::render() {
    renderComponent->render();
}

std::vector<Triangle *> GameObject::getTriangles() {
    return mesh->getTriangles();
}

float4x4 GameObject::getModelMatrix(){
    return m_modelMatrix;
}


void GameObject::renderShadow(Shader *shaderProgram) {
    renderComponent->renderShadow(shaderProgram);
}

void GameObject::addRenderComponent(IRenderComponent* renderer){
    this->renderComponent = renderer;
    components.push_back(renderer);
}

void GameObject::addComponent(IComponent* newComponent) {
    components.push_back(newComponent);
}

void GameObject::update(float dt) {
    for(auto &component : components) {
        component->update(dt);
    }
}

void GameObject::callEvent(EventType type, GameObjectType data){

    switch(type) {
    case EventType::BeforeCollision:
        for (auto &component : components) {
            component->beforeCollision(data);
        }
        break;
    case EventType::DuringCollision:
        for (auto &component : components) {
            component->duringCollision(data);
        }
        break;
    case EventType::AfterCollision:
        for (auto &component : components) {
            component->afterCollision(data);
        }
        break;
    }
}

AABB GameObject::getAABB(){
    AABB* meshAabb = this->mesh->getAABB();
    aabb = multiplyAABBWithModelMatrix(meshAabb, m_modelMatrix);

    return aabb;
}

bool GameObject::isDynamicObject(){
    return dynamicObject;
}

void GameObject::setDynamic(bool isDynamic) {
    dynamicObject = isDynamic;
}

Octree* GameObject::getOctree(){
    return octree;
}

int GameObject::getId() {
    return id;
}