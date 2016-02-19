#include <iostream>
#include "GL\glew.h"
#include "GL\glut.h"
#include "ResourceInitializer.h"
#include "ProjectEquinox.h"

Texture* ResourceInitializer::errorTex = NULL;

Texture* ResourceInitializer::getErrorTexture(){
	if (errorTex == NULL){
		errorTex = new Texture();
		errorTex->loadPNG("../executable/res/textures/error.png");
		errorTex->bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		errorTex->unbind();
	}

	return errorTex;
}

void ResourceInitializer::initializeEntities(){
}