#ifndef TERRAIN_H
#define TERRAIN_H

#include "../Graphics/Texture.h"

class Planet {
public:
	Planet();
	~Planet();
	void generate(int seed, int res);
	void draw();
	void update(int timeDelta);
	Texture* getHeightmap();
	double getHeight();
	void setHeight(double height);
private:
	int seed;
	Texture* heightmap;
	double** heightmapData;
	int heightmapRes;

	double width;
	double height;
	double length;

	double x;
	double y;
	double z;
};

#endif