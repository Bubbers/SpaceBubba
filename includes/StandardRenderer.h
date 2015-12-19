#ifndef BUBBA_3D_STANDARDRENDERER_H
#define BUBBA_3D_STANDARDRENDERER_H

#include "Mesh.h"
#include "float4x4.h"
#include "Shader.h"
#include "IRenderComponent.h"
#include "GameObject.h"

using namespace chag;

class StandardRenderer : public IRenderComponent {
public:
    StandardRenderer();
    StandardRenderer(Mesh*, GameObject*, Shader*);

    void update(float dt);
    void render();
    void renderShadow(Shader *shaderProgram);
private:
    Mesh* mesh;
    GameObject *gameObject;
};


#endif //BUBBA_3D_STANDARDRENDERER_H
