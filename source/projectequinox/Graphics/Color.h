#ifndef COLOR_H
#define COLOR_H

#include <vector>

class Color{
public:
	Color();
	Color(double r, double g, double b);
	Color(double r, double g, double b, double a);
	Color(const Color* col);
	~Color();

	void setColor(double r, double g, double b);
	void setColor(double r, double g, double b, double a);
	void setColor(const Color* col);
	void setRed(double r);
	void setGreen(double g);
	void setBlue(double b);
	void setAlpha(double a);

	double getRed() const;
	double getGreen() const;
	double getBlue() const;
	double getAlpha() const;

	bool equals(const Color* col) const;
	double operator[](int i);
private:
	std::vector<double> color;
	const int R_INDEX = 0;
	const int G_INDEX = 1;
	const int B_INDEX = 2;
	const int A_INDEX = 3;
};

#endif