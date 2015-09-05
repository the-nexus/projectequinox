#ifndef PLANET_H
#define PLANET_H

#include "../Geometry/Point.h"
#include "../Graphics/Color.h"
#include <vector>

class Planet {
public:
	Planet();
	~Planet();
	void generate(int seed, int res);
	void draw();
	void update(int timeDelta);
	double getHeight();
	void setHeight(double height);
	void subdivide();
	void unsubdivide();
private:
	class Cell {
	public:
		Cell();
		Cell(Cell* parentCell);
		Cell(Point* p1, Point* p2, Point* p3, Cell* parentCell);
		~Cell();
		Cell* getSubcell(int n);
		Point* getPoint(int n);
		bool isSubdivided();
		bool hasParentCell();
		Cell* getParentCell();
		void setPoint(int n, Point* p);
		void subdivide();
		void unsubdivide();
		void update(int timeDelta);
		void draw();
	private:
		bool subdivided;
		std::vector<Cell*> subcells;
		std::vector<Point*> points;
		Cell* parentCell;
		Color* color;
	};

	int seed;
	double radius;
	double height;

	double x;
	double y;
	double z;

	std::vector<Cell*> cells;
};

#endif