#ifndef POINT_H
#define POINT_H

class Point{
public:
	Point();
	Point(double x, double y, double z);
	Point(const Point* p);
	~Point();

	double getX() const;
	double getY() const;
	double getZ() const;

	void setCoords(double x, double y, double z);
	void setCoords(const Point* p);
	void setX(double x);
	void setY(double y);
	void setZ(double z);

	bool equals(const Point* p) const;
	void update(int timeDelta);
	void draw();
	void normalize();
private:

	double x;
	double y;
	double z;
};

#endif