#include <iostream>		//For cout and NULL
#include <vector>
#include "Planet.h"
#include "GL/glew.h"
#include "PerlinNoise.h"
#include "Planet.h"

Planet::Planet() {
	width = 100.0;
	length = 100.0;
	height = 0.0;
	x = -width/2.0;
	z = -length/2.0;
	y = 0.0;

	heightmap = new Texture();
	heightmapRes = 1;
	heightmapData = new double*[heightmapRes];
	for (int j = 0; j < heightmapRes; j++) {
		heightmapData[j] = new double[heightmapRes];
		for (int i = 0; i < heightmapRes; i++) {
			heightmapData[j][i] = 0.0;
		}
	}

	points = std::vector<Point*>();
}

Planet::~Planet() {
	for (int j = 0; j < heightmapRes; j++)
		delete heightmapData[j];
	delete heightmapData;
	delete heightmap;
}

void Planet::generate(int seed, int res) {
	Point* p = new Point();
	points.push_back(p);


	/*for (int j = 0; j < heightmapRes; j++)
		delete heightmapData[j];
	delete heightmapData;
	delete heightmap;
	heightmap = new Texture();

	//Size of the texture
	int width = res;
	int height = res;
	heightmapRes = res;

	//heightmap init
	heightmapData = new double*[height];
	for (int j = 0; j < height; j++)
		heightmapData[j] = new double[width];



	//Generate perlin noise
	PerlinNoise* perlin = new PerlinNoise(seed);
	int kk = 0;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			double x = 10.0 * (double)i / (double)width;
			double y = 10.0 * (double)j / (double)height;

			double val = sin(x + 1.0/perlin->noise(x, y, 0.0));
			heightmapData[j][i] = val;
		}
	}

	delete perlin;
	


	//Create the texture buffer
	std::vector<unsigned char> texData = std::vector<unsigned char>(width*height * 4);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			int vecPos = (j*width + i) * 4;
			unsigned char val = (unsigned char)(heightmapData[j][i] * 255);

			texData[vecPos + 0] = val;
			texData[vecPos + 1] = val;
			texData[vecPos + 2] = val;
			texData[vecPos + 3] = 255;
		}
	}



	//Create the texture
	heightmap = new Texture();
	heightmap->createFromBuffer(texData, width, height);*/
}

void Planet::update(int timeDelta) {
}

void Planet::draw() {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();


	//glTranslated(x, y, z);
	//glScaled(width, height, length);
	glColor4d(1.0, 1.0, 1.0, 1.0);

	points[0]->draw();
	



	/*heightmap->bind();

	for (int j = 0; j < length-1; j++) {
		glBegin(GL_TRIANGLE_STRIP);
		{
			int heightmapJ1 = (int)(heightmapRes * j / length);
			int heightmapJ2 = (int)(heightmapRes * (j + 1) / length);
			bool flipped = false;
			for (int i = 0; i < width; i++) {
				int heightmapI = (int)(heightmapRes * i / width);
				double heightTop = height * heightmapData[heightmapJ1][heightmapI];
				double heightBot = height * heightmapData[heightmapJ2][heightmapI];

				if (flipped) {
					glTexCoord2d(i / width, (j + 1) / length);
					glVertex3d((double)i, heightBot, (double)j + 1);
					glTexCoord2d(i / width, j / length);
					glVertex3d((double)i, heightTop, (double)j);
				}
				else {
					glTexCoord2d(i / width, j / length);
					glVertex3d((double)i, heightTop, (double)j);
					glTexCoord2d(i / width, (j + 1) / length);
					glVertex3d((double)i, heightBot, (double)j + 1);
				}
			}
		}
		glEnd();
	}*/
	

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

Texture* Planet::getHeightmap() {
	return heightmap;
}

double Planet::getHeight() {
	return height;
}

void Planet::setHeight(double height) {
	this->height = height;
}