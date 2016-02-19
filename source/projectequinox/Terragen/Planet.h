#ifndef PLANET_H
#define PLANET_H

#include "../Geometry/Point.h"
#include "../Graphics/Color.h"
#include "../Terragen/PerlinNoise.h"
#include <vector>

class Planet {
public:
	Planet();
	~Planet();
	void generate(int seed, int res);
	int draw();
	void update(int timeDelta);
	double getHeight();
	double getRadius();
	void setHeight(double height);
	void subdivide();
	void unsubdivide();
private:
	class Cell {
	public:
		Cell(Planet* parentPlanet);
		Cell(Cell* parentCell, Planet* parentPlanet);
		Cell(Point* p1, Point* p2, Point* p3, Cell* parentCell, Planet* parentPlanet);
		~Cell();
		Cell* getSubcell(int n);
		Point* getPoint(int n);
		bool isSubdivided();
		bool hasParentCell();
		Cell* getParentCell();
		int getLevel();
		void setPoint(int n, Point* p);
		void forceSubdivide();
		void forceUnsubdivide();
		double heightFunction(double x, double y, double z, double frequency, double amplitude, PerlinNoise* numberGenerator);
		void update(int timeDelta);
		int draw();
	private:
		double generateHeight(Point* p);

		int level;
		bool subdivided;
		bool generated;
		std::vector<Cell*> subcells;
		std::vector<Point*> points;
		std::vector<double> heights;
		Cell* parentCell;
		Planet* parentPlanet;
		Color* color;
	};

	PerlinNoise* noiseGenerator;
	double radius;
	double height;

	double x;
	double y;
	double z;

	double yawSpeed;
	double yaw;
	double outerAngle;
	double outerSpeed;
	double outerDist;

	std::vector<Cell*> cells;
};

#endif