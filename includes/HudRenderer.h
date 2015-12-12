//
// Created by johan on 2015-12-12.
//

#ifndef BUBBA_3D_HUDRENDERER_H
#define BUBBA_3D_HUDRENDERER_H

#include "IRenderComponent.h"
#include "GL/glew.h"

class HudRenderer : public IRenderComponent
{
public:
    HudRenderer();
    ~HudRenderer();
    struct HudConfig{
        float speed = 0.0f;
    };

    virtual void render();
    virtual void renderShadow(Shader *shaderProgram);
    virtual void update(float dt);
    void render2DHud(Texture* texture, float4x4 *modelMatrix);
    struct HudConfig* getConfig();

private:
    GLuint m_vaob;
    struct HudConfig* conf;

};

#endif //BUBBA_3D_HUDRENDERER_H
