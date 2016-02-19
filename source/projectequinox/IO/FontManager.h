#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <vector>
#include "../Graphics/Font.h"

class FontManager {
public:
	static FontManager* getInstance();

	bool addFont(char* name, char* fontFilePath);
	Font* getFont(int index);
	Font* getFont(char* name);
	int getFontCount();
private:
	FontManager();
	~FontManager();

	static FontManager* fontManager;

	std::vector<Font*> fonts;
};

#endif