#ifndef __RENDERER_H__
#define __RENDERER_H__

#ifdef WIN32
#include <windows.h>
#endif

#include "Utils.h"
#include "Effects.h"

#define CUBE_MAP_RESOLUTION		512
#define SHADOW_MAP_RESOLUTION	2048

using namespace chag;

class Camera;
class Scene;
class IDrawable;

class Renderer
{
public:
	Renderer(int argc, char *argv[], int width, int height);
	~Renderer();

	void initGL();

	void drawScene(Camera *camera, Scene* scene, float currentTime);
	void start();
	void render();

	void setIdleMethod(void(*idle)(int), int delay);
	void setDisplayMethod(void(*display)(void));

	void swapBuffer() {
		glutSwapBuffers();  // swap front and back buffer. This frame will now be displayed.
		CHECK_GL_ERROR();
	}


	Effects effects;
private:
	int width, height;
	float currentTime;

	Fbo createPostProcessFbo(int width, int height);
	void drawShadowMap(Fbo sbo, float4x4 viewProjectionMatrix, Scene *scene);
	void drawShadowCasters(Shader* shaderProgram, Scene *scene);
	void drawTransparent(Shader* shaderProgram, Scene *scene);
	void setFog(Shader* shaderProgram);
	void setLights(Shader* shaderProgram, Scene *scene);

	Shader* shaderProgram;

	Fbo sbo;
	Camera *cubeMapCameras[6];

	//Drawing
	void drawModel(IDrawable &model, Shader* shaderProgram);
	void drawFullScreenQuad();
	void renderPostProcess();
	void blurImage();


	//Postprocess
	Shader* postFxShader;
	Shader* horizontalBlurShader;
	Shader* verticalBlurShader;
	Shader* cutoffShader;
	Fbo postProcessFbo, horizontalBlurFbo, verticalBlurFbo, cutOffFbo;
};


#endif // __RENDERER_H__
