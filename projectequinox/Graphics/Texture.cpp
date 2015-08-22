#include <iostream>
#include <fstream>
#include "Texture.h"
#include "../IO/IOManager.h"
#include "../Libs/picopng.h"

using namespace std;

Texture::Texture(){
	id = 0;
	width = 0;
	height = 0;
	loaded = false;
}
Texture::~Texture(){
}

void Texture::loadPNG(char* filePath){
	if (!loaded){

		std::vector<unsigned char> in, out;
		unsigned long w, h;

		if (IOManager::readFileToBuffer(filePath, in) == false){
			cout << "Failed to load PNG file to buffer at [" << filePath << "]" << endl;
			return;
		}

		int error = decodePNG(out, w, h, (&in[0]), in.size());
		if (error != 0){
			cout << "Failed to decode PNG at [" << filePath << "] (error code " << error << ")"<< endl;
			return;
		}

		width = w;
		height = h;
		
		/* DEBUG SECTION
		cout << "Tex: W=" << width << ", H=" << height << ", S=" << out.size() << endl;

		for (int i = 0; i < out.size(); i += 4){
			cout << "r=" << (int)(out[i + 0]) << ", g=" << (int)(out[i + 1]) << ", b=" << (int)(out[i + 2]) << ", a=" << (int)(out[i + 3]) << endl;
		}
		*/

		glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_2D, id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

		glBindTexture(GL_TEXTURE_2D, 0);

		cout << "Texture loaded at [" << filePath << "]" << endl;
		loaded = true;
	}
	else{
		cout << "A texture already exists in this object" << endl;
	}
}
void Texture::bind(){
	if (loaded){
		glBindTexture(GL_TEXTURE_2D, id);
	}
}
void Texture::unbind(){
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::getWidth(){
	return 0;
}
int Texture::getHeight(){
	return 0;
}
bool Texture::isLoaded(){
	return loaded;
}