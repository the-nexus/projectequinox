#include "Color.h"

/* -----------------------------------------
 *		Construction / Destruction
 * -----------------------------------------
 */

Color::Color(){
	color = std::vector<double>(4);
	color[R_INDEX] = 1.0;	//Red
	color[G_INDEX] = 1.0;	//Green
	color[B_INDEX] = 1.0;	//Blue
	color[A_INDEX] = 1.0;	//Alpha
}

Color::Color(double r, double g, double b){
	color = std::vector<double>(4);
	color[R_INDEX] = r;		//Red
	color[G_INDEX] = g;		//Green
	color[B_INDEX] = b;		//Blue
	color[A_INDEX] = 1.0;	//Alpha
}

Color::Color(double r, double g, double b, double a){
	color = std::vector<double>(4);
	color[R_INDEX] = r;		//Red
	color[G_INDEX] = g;		//Green
	color[B_INDEX] = b;		//Blue
	color[A_INDEX] = a;		//Alpha
}

Color::Color(const Color* col){
	color = std::vector<double>(4);
	color[R_INDEX] = col->color[R_INDEX];	//Red
	color[G_INDEX] = col->color[G_INDEX];	//Green
	color[B_INDEX] = col->color[B_INDEX];	//Blue
	color[A_INDEX] = col->color[A_INDEX];	//Alpha
}

Color::~Color(){
}


/* -----------------------------------------
 *					Getters
 * -----------------------------------------
 */

double Color::getRed() const {
	return color[R_INDEX];
}

double Color::getGreen() const {
	return color[G_INDEX];
}

double Color::getBlue() const {
	return color[B_INDEX];
}

double Color::getAlpha() const {
	return color[A_INDEX];
}

/* -----------------------------------------
 *					Setters
 * -----------------------------------------
 */

void Color::setColor(double r, double g, double b){
	color[R_INDEX] = r;	//Red
	color[G_INDEX] = g;	//Green
	color[B_INDEX] = b;	//Blue
}

void Color::setColor(double r, double g, double b, double a){
	color[R_INDEX] = r;	//Red
	color[G_INDEX] = g;	//Green
	color[B_INDEX] = b;	//Blue
	color[A_INDEX] = a;	//Alpha
}

void Color::setColor(const Color* col){
	color[R_INDEX] = col->color[R_INDEX];	//Red
	color[G_INDEX] = col->color[G_INDEX];	//Green
	color[B_INDEX] = col->color[B_INDEX];	//Blue
	color[A_INDEX] = col->color[A_INDEX];	//Alpha
}

void Color::setRed(double r){
	color[R_INDEX] = r;
}

void Color::setGreen(double g){
	color[G_INDEX] = g;
}

void Color::setBlue(double b){
	color[B_INDEX] = b;
}

void Color::setAlpha(double a){
	color[A_INDEX] = a;
}

/* -----------------------------------------
 *				Other methods
 * -----------------------------------------
 */

double Color::operator[](int i){
	return color[i];
}

bool Color::equals(const Color* col) const {
	for (int i = 0; i < 4; i++){
		if (color[i] != col->color[i])
			return false;
	}
	return true;
}