#include <iostream>		//For cout and NULL
#include "Planet.h"
#include "GL/glew.h"
#include "PerlinNoise.h"
#include "Planet.h"

Planet::Planet() {
	radius = 1.0;
	x = 0.0;
	z = 0.0;
	y = 0.0;

	cells = std::vector<Cell*>();
}

Planet::~Planet() {
	for (unsigned int i = 0; i < cells.size(); i++)
		delete cells[i];
}

void Planet::generate(int seed, int res) {
	/*
				*
			   /g\				g: gamma
			r /   \ r			t: theta
			 /t   t\			r: radius
			*-------*			q: edge length
				q
	*/

	const double PI = 3.141593;

	double gamma = 72.0 / 180.0 * PI;
	double theta = 54.0 / 180.0 * PI;

	double edgeLength = sin(gamma) / sin(theta);

	std::vector<Point*> points = std::vector<Point*>();


}

void Planet::update(int timeDelta) {
	for (unsigned int i = 0; i < cells.size(); i++)
		cells[i]->update(timeDelta);
}

void Planet::draw() {
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();


	glTranslated(x, y, z);
	glScaled(radius, radius, radius);
	glColor4d(1.0, 1.0, 1.0, 1.0);
	
	for (unsigned int i = 0; i < cells.size(); i++)
		cells[i]->draw();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

double Planet::getHeight() {
	return height;
}

void Planet::setHeight(double height) {
	this->height = height;
}



//////////////////////////////////////////////////////////////////////////////////////////
//		CELLS
//////////////////////////////////////////////////////////////////////////////////////////

Planet::Cell::Cell() {
	subcells = std::vector<Cell*>();
	points = std::vector<Point*>();
	points.push_back(new Point());
	points.push_back(new Point());
	points.push_back(new Point());
	this->subdivided = false;
	this->parentCell = NULL;
}
Planet::Cell::Cell(Cell* parentCell) {
	subcells = std::vector<Cell*>();
	points = std::vector<Point*>();
	points.push_back(new Point());
	points.push_back(new Point());
	points.push_back(new Point());
	this->subdivided = false;
	this->parentCell = parentCell;
}
Planet::Cell::Cell(Point* p1, Point* p2, Point* p3, Cell*) {
	points = std::vector<Point*>();
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	this->subdivided = false;
	this->parentCell = parentCell;
}
Planet::Cell::~Cell() {
	for (unsigned int i = 0; i < subcells.size(); i++)
		delete subcells[i];
	for (unsigned int i = 0; i < points.size(); i++)
		delete points[i];
}

// GETTERS

Point* Planet::Cell::getPoint(int n) {
	return points[n];
}
Planet::Cell* Planet::Cell::getSubcell(int n) {
	return subcells[n];
}
Planet::Cell* Planet::Cell::getParentCell() {
	return parentCell;
}
bool Planet::Cell::isSubdivided() {
	return subdivided;
}
bool Planet::Cell::hasParentCell() {
	return (parentCell != NULL);
}

// SETTERS

void Planet::Cell::setPoint(int n, Point* p) {
	delete points[n];
	points[n] = p;
}

// OTHERS

void Planet::Cell::subdivide() {

}
void Planet::Cell::update(int timeDelta) {
	if (subdivided) {
		for (unsigned int i = 0; i < subcells.size(); i++)
			subcells[i]->update(timeDelta);
	}
	else {
		for (unsigned int i = 0; i < points.size(); i++)
			points[i]->update(timeDelta);
	}
}
void Planet::Cell::draw() {
	if (subdivided) {
		for (unsigned int i = 0; i < subcells.size(); i++)
			subcells[i]->draw();
	}
	else {
		for (unsigned int i = 0; i < points.size(); i++)
			points[i]->draw();
	}
}