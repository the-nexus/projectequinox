#include <iostream>
#include "Font.h"

Font::Font() {
	init();
}

Font::Font(char* name, char* fontFilePath) {
	init();
	create(name, fontFilePath);
}

Font::~Font() {

}

void Font::init() {
	name = "";
	fontTex = NULL;
	created = false;
}

bool Font::isCreated() {
	return created;
}

void Font::create(char* name, char* fontFilePath) {
	if (!created) {
		this->name = name;
		fontTex = new Texture();
		fontTex->loadPNG(fontFilePath);

		if (!fontTex->isLoaded()) {
			delete fontTex;
			this->name = "";
			created = false;
		}
		else {
			fontTex->bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			fontTex->unbind();
			created = true;
		}
	}
}

char* Font::getName() {
	return name;
}

Texture* Font::getFontTex() {
	return fontTex;
}