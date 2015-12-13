//
// Created by johan on 2015-12-13.
//

#ifndef BUBBA_3D_POSTPROCESSINGRENDERER_H
#define BUBBA_3D_POSTPROCESSINGRENDERER_H

#include "IRenderComponent.h"

class PostProcessingRenderer : IRenderComponent {
public:
    PostProcessingRenderer();
    ~PostProcessingRenderer();
    virtual void render();
    virtual void renderShadow(Shader *shaderProgram);

private:
};

#endif //BUBBA_3D_POSTPROCESSINGRENDERER_H
