#include <iostream>
#include "GL/glew.h"
#include "Graphic.h"
#include "PerlinNoise.h"

Graphic::Graphic() {
	generate(0);
	width = 100;
	height = 50;
	x = -width/2.0;
	y = -height/2.0;
	z = 0.0;
}

Graphic::~Graphic() {
	delete mapData;
	std::cout << "Deleted graphic entity" << std::endl;
}

void Graphic::generate(int size) {
	if (mapData != NULL)
		delete mapData;
	this->mapSize = size;
	this->mapData = new double[size];
	PerlinNoise::generateNoise1D(mapData, size, 0.5);
	drawStyle = LINES;
}

void Graphic::update(int timeDelta) {

}

void Graphic::draw() {
	glPushMatrix();
	glTranslated(x, y, z);
	glScaled(width, height, 1.0);

	glColor3d(1.0, 1.0, 1.0);
	if (drawStyle == POINTS) {
		glPointSize(1.0);
		for (int i = 0; i < mapSize; i++) {
			glBegin(GL_POINTS);
			{
				glVertex3d((double)i / (double)mapSize, mapData[i], 0.0);
			}
			glEnd();
		}
	}
	else if (drawStyle == LINES) {
		glBegin(GL_LINE_STRIP);
		{
			for (int i = 1; i < mapSize; i++)
				glVertex3d((double)i / (double)mapSize, mapData[i], 0.0);
		}
		glEnd();
	}

	glPopMatrix();
}