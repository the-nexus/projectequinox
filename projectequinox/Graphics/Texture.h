#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include "GL/glew.h"
#include "Color.h"

class Texture{
public:
	Texture();
	~Texture();
	void loadPNG(char* filePath);
	void createFromBuffer(std::vector<unsigned char> buffer, int width, int height);
	void bind();
	void unbind();
	int getWidth();
	int getHeight();
	bool isLoaded();
	Color* getPixelColor(double x, double y);
private:
	std::vector<unsigned char> imageData;
	GLuint id;
	int width;
	int height;
	bool loaded;
};

#endif