#ifndef __UTILS_H__
#define __UTILS_H__

#include <GL/freeglut.h>
#include "Shader.h"

//*****************************************************************************
//	Frame buffer objects
//*****************************************************************************
struct Fbo{
	GLuint id;
	GLuint texture;
	Shader* shaderProgram;
	GLuint depthbuffer;

	int width, height;
};


static float degreeToRad(float degree) {
	return (float) (degree * M_PI / 180);
}

static float radToDegree(float rad) {
	return (float) (rad * 180 / M_PI);
}

static Fbo createPostProcessFbo(int width, int height) {
	Fbo fbo;

	fbo.width = width;
	fbo.height = height;

	glGenFramebuffers(1, &fbo.id);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.id);

	glGenTextures(1, &fbo.texture);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, fbo.texture);

	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE_ARB, fbo.texture, 0);

	glGenRenderbuffers(1, &fbo.depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo.depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo.depthbuffer);

	return fbo;
}

static void drawFullScreenQuad()
{
	static GLuint vertexArrayObject = 0;
	static int nofVertices = 4;

	// do this initialization first time the function is called... somewhat dodgy, but works for demonstration purposes
	if (vertexArrayObject == 0)
	{
		glGenVertexArrays(1, &vertexArrayObject);
		static const float2 positions[] = {
				/*		{ -1.0f, -1.0f },
                        { 1.0f, -1.0f },
                        { 1.0f, 1.0f },
                        { -1.0f, 1.0f },*/
				-1.0f, -1.0f,
				1.0f, 1.0f,
				-1.0f, 1.0f,

				-1.0f, -1.0f,
				1.0f, -1.0f,
				1.0f, 1.0f
		};
		createAddAttribBuffer(vertexArrayObject, positions, sizeof(positions), 0, 2, GL_FLOAT);
		GLuint pos_vbo;
		glGenBuffers(1, &pos_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

		glBindVertexArray(vertexArrayObject);
		glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0 );
		glEnableVertexAttribArray(0);
		CHECK_GL_ERROR();
	}

	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}


#endif