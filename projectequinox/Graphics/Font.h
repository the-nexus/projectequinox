#ifndef FONT_H
#define FONT_H

#include "Texture.h"

class Font {
public:
	Font();
	Font(char* name, char* fontFilePath);
	~Font();

	bool isCreated();
	void create(char* name, char* fontFilePath);
	char* getName();
	Texture* getFontTex();
private:
	void init();

	bool created;
	char* name;
	Texture* fontTex;
};

#endif