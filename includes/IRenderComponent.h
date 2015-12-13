#ifndef BUBBA_3D_RENDERCOMPONENT_H
#define BUBBA_3D_RENDERCOMPONENT_H

#include "IComponent.h"
#include "Utils.h"
#include <string>
#include "ResourceManager.h"
#include "constants.h"

using namespace std;

class IRenderComponent : public IComponent {
public:
    IRenderComponent(int width, int height) {
        this->width = width;
        this->height = height;
        frameBufferObject = createPostProcessFbo(width, height);

    }
    virtual void render() = 0;
    virtual void renderShadow(Shader *shaderProgram) = 0;

    const void blitToDefaultFrameBuffer() {


        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferObject.id);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
                          GL_NEAREST);

        /*



        Shader* frameBufferChangeShader = ResourceManager::getShader(FRAME_BUFFER_CHANGE_SHADER_NAME);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        frameBufferChangeShader->use();
        frameBufferChangeShader->setUniform1i("textureImage", 0);

        glEnable(GL_DEPTH_TEST);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_RECTANGLE_ARB, frameBufferObject.texture);

        drawFullScreenQuad();*/
    }


protected:
    Shader *shaderProgram;

    Fbo frameBufferObject;
    int height = 0;
    int width = 0;
    IRenderComponent() {};

private:

};


#endif //BUBBA_3D_RENDERCOMPONENT_H
