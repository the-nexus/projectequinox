#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "../Graphics/Font.h"

class TextLabel {
public:
	TextLabel();
	TextLabel(Font* font, double size);
	TextLabel(Font* font, double size, std::string text, double x, double y);
	~TextLabel();
	double getSize();
	double getWidth();
	double getX();
	double getY();
	void setText(std::string text);
	void setPosition(double x, double y);
	void setFont(Font* font);
	void setSize(double size);
	void update(int timeDelta);
	void draw();
private:
	void getCharMarks(char c, double &startX, double &startY, double &endX, double &endY);

	Font* font;
	std::string text;
	double x;
	double y;
	double size;
};

#endif