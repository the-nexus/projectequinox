#include <iostream>
#include "FontManager.h"

FontManager* FontManager::fontManager = NULL;

FontManager* FontManager::getInstance() {
	if (FontManager::fontManager == NULL)
		FontManager::fontManager = new FontManager();
	return FontManager::fontManager;
}

FontManager::FontManager() {
	fonts = std::vector<Font*>();
}

FontManager::~FontManager() {

}

bool FontManager::addFont(char* name, char* fontFilePath) {
	Font* newFont = new Font(name, fontFilePath);

	if (newFont->isCreated()) {
		fonts.push_back(newFont);
		return true;
	}
	else {
		delete newFont;
		return false;
	}
}

Font* FontManager::getFont(int index) {
	return fonts[index];
}

Font* FontManager::getFont(char* name) {
	for (unsigned int i = 0; i < fonts.size(); i++) {
		if (strcmp(name, fonts[i]->getName()) == 0)
			return fonts[i];
	}
	return NULL;
}

int FontManager::getFontCount() {
	return fonts.size();
}