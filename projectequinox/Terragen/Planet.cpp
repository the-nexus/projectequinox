#include <iostream>		//For cout and NULL
#include "Planet.h"
#include "GL/glew.h"
#include "PerlinNoise.h"
#include "Planet.h"

Planet::Planet() {
	radius = 50;
	x = 0.0;
	z = 0.0;
	y = 0.0;

	yaw = 0.0;
	yawSpeed = 0.01;

	cells = std::vector<Cell*>();
}

Planet::~Planet() {
	for (unsigned int i = 0; i < cells.size(); i++)
		delete cells[i];
}

void Planet::generate(int seed, int res) {
	/*																			
				*				g: gamma						    q	  ---*
			   /g\				t: theta							  ----   |	t
			r /   \ r			r: radius (= 1.0)				  ----		 |
			 /t   t\			q: edge length					*------------*
			*-------*			t: radius tangent					   r
				q
	*/

	const double PI = 3.141593;
	double radius = 1.0;

	double gamma = 72.0 / 180.0 * PI;
	double theta = 54.0 / 180.0 * PI;

	double edgeLength = radius * sin(gamma) / sin(theta);
	double pentagonHeight = radius - sqrt(pow(edgeLength, 2.0) + pow(radius, 2.0));

	std::vector<Point*> points = std::vector<Point*>();

	points.push_back(new Point(0.0, radius, 0.0));
	for (int i = 0; i < 5; i++) {
		double angle = gamma * i;
		Point* p = new Point(sin(angle), pentagonHeight, cos(angle));
		p->normalize();
		points.push_back(p);
		angle = gamma * (i + 0.5);
		p = new Point(sin(angle), -pentagonHeight, cos(angle));
		p->normalize();
		points.push_back(p);
	}
	points.push_back(new Point(0.0, -radius, 0.0));
	
	for (int face = 0; face < 20; face++) {
		Point* p1;
		Point* p2;
		Point* p3;

		if (face < 5) {
			p1 = points[0];
			p3 = points[((face + 1) * 2 + 1) % 10 + 1];
			p2 = points[(face * 2 + 1) % 10 + 1];
		}
		else if (face < 15) {
			if (face % 2 == 0) {
				p1 = points[((face - 5) % 10) + 1];
				p2 = points[((face - 5 + 1) % 10) + 1];
				p3 = points[((face - 5 + 2) % 10) + 1];
			}
			else {
				p1 = points[((face - 5) % 10) + 1];
				p3 = points[((face - 5 + 1) % 10) + 1];
				p2 = points[((face - 5 + 2) % 10) + 1];
			}
		}
		else {
			p1 = points[(face * 2 + 1) % 10];
			p2 = points[11];
			p3 = points[((face + 1) * 2 + 1) % 10];
		}

		cells.push_back(new Cell(p1, p2, p3, NULL));
	}
}

double Planet::getHeight() {
	return height;
}

void Planet::setHeight(double height) {
	this->height = height;
}


void Planet::subdivide() {
	for (unsigned int i = 0; i < cells.size(); i++)
		cells[i]->subdivide();
}

void Planet::unsubdivide() {
	for (unsigned int i = 0; i < cells.size(); i++)
		cells[i]->unsubdivide();
}

void Planet::update(int timeDelta) {
	for (unsigned int i = 0; i < cells.size(); i++)
		cells[i]->update(timeDelta);

	yaw += yawSpeed * timeDelta;
}

int Planet::draw() {
	int polyDrawn = 0;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glPushMatrix();

	GLfloat materialEmission[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat materialAmbient[] = { 0.15, 0.15, 0.15, 1.0 };
	GLfloat materialDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat materialSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat materialShininess[] = { 100.0 };

	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

	glTranslated(x, y, z);
	glRotated(yaw, 0.0, 1.0, 0.0);
	glScaled(radius, radius, radius);
	glColor4d(1.0, 1.0, 1.0, 1.0);

	for (unsigned int i = 0; i < cells.size(); i++)
		polyDrawn += cells[i]->draw();

	glPopMatrix();
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	return polyDrawn;
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
	this->color = new Color(rand() % 100 /100.0, rand() % 100 / 100.0, rand() % 100 / 100.0);
}
Planet::Cell::Cell(Cell* parentCell) {
	subcells = std::vector<Cell*>();
	points = std::vector<Point*>();
	points.push_back(new Point());
	points.push_back(new Point());
	points.push_back(new Point());
	this->subdivided = false;
	this->parentCell = parentCell;
	this->color = new Color(rand() % 100 / 100.0, rand() % 100 / 100.0, rand() % 100 / 100.0);
}
Planet::Cell::Cell(Point* p1, Point* p2, Point* p3, Cell*) {
	points = std::vector<Point*>();
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	this->subdivided = false;
	this->parentCell = parentCell;
	this->color = new Color(rand() % 100 / 100.0, rand() % 100 / 100.0, rand() % 100 / 100.0);
}
Planet::Cell::~Cell() {
	for (unsigned int i = 0; i < subcells.size(); i++)
		delete subcells[i];
	delete color;
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
	if (subdivided) {
		for (unsigned int i = 0; i < subcells.size(); i++)
			subcells[i]->subdivide();
	}
	else {
		//Establish the middle point between the two points
		Point* p1 = points[0];
		Point* p2 = points[1];
		Point* p3 = points[2];

		Point* subP1 = new Point(
			p1->getX() + ((p2->getX() - p1->getX()) / 2.0),
			p1->getY() + ((p2->getY() - p1->getY()) / 2.0),
			p1->getZ() + ((p2->getZ() - p1->getZ()) / 2.0)
			);
		Point* subP2 = new Point(
			p2->getX() + ((p3->getX() - p2->getX()) / 2.0),
			p2->getY() + ((p3->getY() - p2->getY()) / 2.0),
			p2->getZ() + ((p3->getZ() - p2->getZ()) / 2.0)
			);
		Point* subP3 = new Point(
			p3->getX() + ((p1->getX() - p3->getX()) / 2.0),
			p3->getY() + ((p1->getY() - p3->getY()) / 2.0),
			p3->getZ() + ((p1->getZ() - p3->getZ()) / 2.0)
			);

		//Normalize to unit vector
		subP1->normalize();
		subP2->normalize();
		subP3->normalize();

		//Create the subcells from the subpoints

		subcells.push_back(new Cell(p1, subP1, subP3, this));		//Top
		subcells.push_back(new Cell(subP1, p2, subP2, this));		//Left
		subcells.push_back(new Cell(subP3, subP2, p3, this));		//Right
		subcells.push_back(new Cell(subP1, subP2, subP3, this));	//Middle

		subdivided = true;
	}
}
void Planet::Cell::unsubdivide() {
	if (!subdivided)
		return;

	bool hasSubdividedSubcell = false;
	for (int i = 0; i < subcells.size(); i++) {
		if (subcells[i]->isSubdivided())
			hasSubdividedSubcell = true;
	}
	if (hasSubdividedSubcell) {
		for (int i = 0; i < subcells.size(); i++) {
			subcells[i]->unsubdivide();
		}
	}
	else {
		for (int i = 0; i < subcells.size(); i++) {
			delete subcells[i];
		}
		subdivided = false;
		subcells.resize(0);
	}
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
int Planet::Cell::draw() {
	int polyDrawn = 0;

	if (subdivided) {
		for (unsigned int i = 0; i < subcells.size(); i++)
			polyDrawn += subcells[i]->draw();
	}
	else {
		glColor4d(color->getRed(), color->getGreen(), color->getBlue(), color->getAlpha());
		//glColor4d(1.0, 1.0, 1.0, 1.0);
		glBegin(GL_TRIANGLES);
		for (unsigned int i = 0; i < points.size(); i++) {
			//points[i]->draw();
			Point* p = points[i];
			glNormal3d(p->getX(), p->getY(), p->getZ());
			glVertex3d(p->getX(), p->getY(), p->getZ());
		}
		glEnd();
		polyDrawn++;
	}

	return polyDrawn;
}

