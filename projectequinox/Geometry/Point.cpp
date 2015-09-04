#include "Point.h"
#include "GL/glew.h"
#include <iostream>

/* -----------------------------------------
 *		Construction / Destruction
 * -----------------------------------------
 */

Point::Point() {
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
}

Point::Point(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
Point::Point(const Point* p) {
	this->x = p->x;
	this->y = p->y;
	this->z = p->z;
}

Point::~Point() {

}

/* -----------------------------------------
 *					Getters
 * -----------------------------------------
 */

double Point::getX() const {
	return this->x;
}
double Point::getY() const {
	return this->y;
}
double Point::getZ() const {
	return this->z;
}

/* -----------------------------------------
 *					Setters
 * -----------------------------------------
 */

void Point::setCoords(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void Point::setCoords(const Point* p) {
	this->x = p->x;
	this->y = p->y;
	this->z = p->z;
}

void Point::setX(double x) {
	this->x = x;
}

void Point::setY(double y) {
	this->y = y;
}

void Point::setZ(double z) {
	this->z = z;
}

/* -----------------------------------------
 *				Other methods
 * -----------------------------------------
 */

bool Point::equals(const Point* p) const {
	if (this->x != p->x)
		return false;
	else if (this->y != p->y)
		return false;
	else if (this->z != p->z)
		return false;
	else
		return true;
}

void Point::update(int timeDelta) {

}

void Point::draw() {
	glPushMatrix();
	glColor4d(1.0, 1.0, 0.0, 1.0);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	{
		glVertex3d(x, y, z);
	}
	glEnd();
	glPopMatrix();
}