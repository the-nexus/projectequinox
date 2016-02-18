#include <iostream>		//For cout and NULL
#include "GL/glew.h"
#include "Planet.h"
#include "Planet.h"

Planet::Planet() {
	radius = 20.0;
	height = 2.0;
	x = 0.0;
	z = 0.0;
	y = 0.0;

	yaw = 0.0;
	yawSpeed = -0.01;

	outerAngle = 0.0;
	outerSpeed = 0.005;
	outerDist = 50.0;

	cells = std::vector<Cell*>();
	noiseGenerator = new PerlinNoise();
}

Planet::~Planet() {
	for (unsigned int i = 0; i < cells.size(); i++)
		delete cells[i];
	delete noiseGenerator;
}

void Planet::generate(int seed, int res) {
	if (seed != noiseGenerator->getSeed()) {
		delete noiseGenerator;
		noiseGenerator = new PerlinNoise(seed);
	}

	/*																			
				*				g: gamma						    q	  ---*
			   /g\				t: theta							  ----   |	t
			r /   \ r			r: radius (= 1.0)				  ----		 |
			 /t   t\			q: edge length					*------------*
			*-------*			t: radius tangent					   r
				q
	*/

	const double PI = 3.14159;
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

		cells.push_back(new Cell(p1, p2, p3, NULL, this));
	}
}

double Planet::getRadius() {
	return radius;
}

double Planet::getHeight() {
	return height;
}

void Planet::setHeight(double height) {
	this->height = height;
}


void Planet::subdivide() {
	for (unsigned int i = 0; i < cells.size(); i++)
		cells[i]->forceSubdivide();
}

void Planet::unsubdivide() {
	for (unsigned int i = 0; i < cells.size(); i++)
		cells[i]->forceUnsubdivide();
}

void Planet::update(int timeDelta) {
	for (unsigned int i = 0; i < cells.size(); i++)
		cells[i]->update(timeDelta);

	yaw += yawSpeed * timeDelta;
	if (yaw >= 360.0)
		yaw -= 360.0;
	outerAngle += outerSpeed * timeDelta;
	if (outerAngle >= 360.0)
		outerAngle -= 360.0;
}

int Planet::draw() {
	int polyDrawn = 0;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glPushMatrix();

	GLfloat materialEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat materialAmbient[] = { 0.15f, 0.15f, 0.15f, 1.0f };
	GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat materialShininess[] = { 100.0f };

	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);

	glTranslated(x, y, z);
	glRotated(outerAngle, 0.0, 1.0, 0.0);
	glTranslated(0.0, 0.0, outerDist);
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

Planet::Cell::Cell(Planet* parentPlanet) {
	subcells = std::vector<Cell*>();
	points = std::vector<Point*>();
	points.push_back(new Point());
	points.push_back(new Point());
	points.push_back(new Point());
	this->subdivided = false;
	this->color = new Color(rand() % 100 /100.0, rand() % 100 / 100.0, rand() % 100 / 100.0);
	this->parentPlanet = parentPlanet;
	this->parentCell = NULL;
	this->level = 0;

	heights = std::vector<double>();
	heights.push_back(generateHeight(points[0]));
	heights.push_back(generateHeight(points[1]));
	heights.push_back(generateHeight(points[2]));
}
Planet::Cell::Cell(Cell* parentCell, Planet* parentPlanet) {
	subcells = std::vector<Cell*>();
	points = std::vector<Point*>();
	points.push_back(new Point());
	points.push_back(new Point());
	points.push_back(new Point());
	this->subdivided = false;
	this->color = new Color(rand() % 100 / 100.0, rand() % 100 / 100.0, rand() % 100 / 100.0);
	this->parentPlanet = parentPlanet;
	this->parentCell = parentCell;
	if(parentCell == NULL)
		this->level = 0;
	else
		this->level = parentCell->level + 1;

	heights = std::vector<double>();
	heights.push_back(generateHeight(points[0]));
	heights.push_back(generateHeight(points[1]));
	heights.push_back(generateHeight(points[2]));
}
Planet::Cell::Cell(Point* p1, Point* p2, Point* p3, Cell* parentCell, Planet* parentPlanet) {
	points = std::vector<Point*>();
	points.push_back(p1);
	points.push_back(p2);
	points.push_back(p3);
	this->subdivided = false;
	this->color = new Color(rand() % 100 / 100.0, rand() % 100 / 100.0, rand() % 100 / 100.0);
	this->parentPlanet = parentPlanet;
	this->parentCell = parentCell;
	if (parentCell == NULL)
		this->level = 0;
	else
		this->level = parentCell->level + 1;

	heights = std::vector<double>();
	heights.push_back(generateHeight(points[0]));
	heights.push_back(generateHeight(points[1]));
	heights.push_back(generateHeight(points[2]));
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
int Planet::Cell::getLevel() {
	return level;
}

// SETTERS

void Planet::Cell::setPoint(int n, Point* p) {
	delete points[n];
	points[n] = p;
}

// OTHERS

void Planet::Cell::forceSubdivide() {
	if (subdivided) {
		for (unsigned int i = 0; i < subcells.size(); i++)
			subcells[i]->forceSubdivide();
	}
	else {
		if (!generated) {
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

			subcells.push_back(new Cell(p1, subP1, subP3, this, this->parentPlanet));		//Top
			subcells.push_back(new Cell(subP1, p2, subP2, this, this->parentPlanet));		//Left
			subcells.push_back(new Cell(subP3, subP2, p3, this, this->parentPlanet));		//Right
			subcells.push_back(new Cell(subP1, subP2, subP3, this, this->parentPlanet));	//Middle

			generated = true;
		}
		subdivided = true;
	}
}
void Planet::Cell::forceUnsubdivide() {
	if (!subdivided)
		return;

	bool hasSubdividedSubcell = false;
	for (unsigned int i = 0; i < subcells.size(); i++) {
		if (subcells[i]->isSubdivided())
			hasSubdividedSubcell = true;
	}
	if (hasSubdividedSubcell) {
		for (unsigned int i = 0; i < subcells.size(); i++) {
			subcells[i]->forceUnsubdivide();
		}
	}
	else {
		/*for (unsigned int i = 0; i < subcells.size(); i++) {
			delete subcells[i];
		}
		subcells.resize(0);
		generated = false;*/
		subdivided = false;
	}
}

double Planet::Cell::heightFunction(double x, double y, double z, double frequency, double amplitude, PerlinNoise* numberGenerator) {
	double height = 0.0;

	//Basic asteroid
	height = sin(1.0 / numberGenerator->noise(x * frequency, y * frequency, z*frequency)) * amplitude;
	//Test
	height = ((x*0) + (y*0.9) + (z*0.1) + 0.1 * numberGenerator->noise(x * frequency, y * frequency, z * frequency));
	height = abs(sin(height * 3.14159)) * amplitude;
	//std::cout << height << std::endl;

	return height;
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
		//glColor4d(color->getRed(), color->getGreen(), color->getBlue(), color->getAlpha());
		glColor4d(0.5, 0.5, 0.5, 1.0);
		glBegin(GL_TRIANGLES);
		for (unsigned int i = 0; i < points.size(); i++) {
			//points[i]->draw();
			Point* p = points[i];
			double h = heights[i] * parentPlanet->height / parentPlanet->radius;
			double x = p->getX() * (1.0 + h);
			double y = p->getY() * (1.0 + h);
			double z = p->getZ() * (1.0 + h);
			glNormal3d(x, y, z);
			glVertex3d(x, y, z);
		}
		glEnd();
		polyDrawn++;
	}

	return polyDrawn;
}

double Planet::Cell::generateHeight(Point* p) {
	PerlinNoise* ng = parentPlanet->noiseGenerator;
	double x = p->getX();
	double y = p->getY();
	double z = p->getZ();
	double height = 0.0;

	int octaves = 16;
	double persistence = 0.5;

	
	for (int i = 0; i < octaves; i++) {
		double frequency = 2 ^ i;
		double amplitude = pow(persistence, i);

		height += heightFunction(x, y, z, frequency, amplitude, ng);	//ASTEROID
	}

	double extraNoise = ng->noise(x, y, z) * 20;
	extraNoise = (extraNoise - (int)extraNoise);

	height -= extraNoise;

	if (height > 1.0)
		height = 1.0;
	else if (height < 0.0)
		height = 0.0;

	return height;
}

