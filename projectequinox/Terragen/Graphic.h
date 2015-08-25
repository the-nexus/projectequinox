#ifndef GRAPHIC_H
#define GRAPHIC_H

class Graphic {
public:
	Graphic();
	~Graphic();
	void generate(int size);
	void update(int timeDelta);
	void draw();
private:
	bool generated;
	int mapSize;
	double* mapData;

	double x;
	double y;
	double z;
	double width;
	double height;

	enum DrawStyle {
		POINTS, LINES
	};
	DrawStyle drawStyle;
};

#endif