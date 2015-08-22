#include <iostream>
#include "TextLabel.h"
#include "../Core/ProjectEquinox.h"
#include "../Core/ResourceInitializer.h"

TextLabel::TextLabel() {
	this->font = NULL;
	this->text = "";
	this->x = 0.0;
	this->y = 0.0;
	this->size = 32.0;
}

TextLabel::TextLabel(Font* font, double size) {
	this->font = font;
	this->text = "";
	this->x = 0.0;
	this->y = 0.0;
	this->size = size;
}

TextLabel::TextLabel(Font* font, double size, std::string text, double x, double y) {
	this->font = font;
	this->text = text;
	this->x = x;
	this->y = y;
	this->size = size;
}

TextLabel::~TextLabel() {

}

double TextLabel::getSize() {
	return size;
}

double TextLabel::getWidth() {
	return size*text.size();
}

void TextLabel::setText(std::string text) {
	this->text = text;
}

void TextLabel::setPosition(double x, double y) {
	this->x = x;
	this->y = y;
}

void TextLabel::setFont(Font* font) {
	this->font = font;
}

void TextLabel::setSize(double size) {
	this->size = size;
}

void TextLabel::update(int timeDelta) {

}

void TextLabel::draw() {
	ProjectEquinox::getInstance()->setViewToOrtho();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glColor4d(1.0, 1.0, 1.0, 1.0);
	if (font != NULL)
		font->getFontTex()->bind();

	glPushMatrix();

	glTranslated(x, y, 0.0);
	glScaled(size, size, size);

	for (unsigned int i = 0; i < text.size(); i++) {
		glPushMatrix();

		glTranslated((double)i, 0.0, 0.0);
		
		double startX;
		double startY;
		double endX;
		double endY;
		getCharMarks(text.at(i), startX, startY, endX, endY);
		
		glBegin(GL_QUADS);
		{
			glTexCoord2d(startX, startY);
			glVertex2d(0.0, 0.0);
			glTexCoord2d(endX, startY);
			glVertex2d(1.0, 0.0);
			glTexCoord2d(endX, endY);
			glVertex2d(1.0, 1.0);
			glTexCoord2d(startX, endY);
			glVertex2d(0.0, 1.0);
		}
		glEnd();

		glPopMatrix();
	}

	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	ProjectEquinox::getInstance()->setViewToPerspective();
}

void TextLabel::getCharMarks(char c, double &startX, double &startY, double &endX, double &endY) {
	int charID = (int)c;
	startX = (double)(charID % 16) / 16.0;
	startY = (double)(charID / 16) / 16.0;
	endX = (double)((charID % 16) + 1) / 16.0;
	endY = (double)((charID / 16) + 1) / 16.0;
}