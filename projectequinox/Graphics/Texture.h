#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"

class Texture{
public:
	Texture();
	~Texture();
	void loadPNG(char* filePath);
	void bind();
	void unbind();
	int getWidth();
	int getHeight();
	bool isLoaded();
private:
	GLuint id;
	int width;
	int height;
	bool loaded;
};

#endif