#include <iostream>
#include <random>		//For Random Generator with seed
#include <algorithm>	//For Shuffle
#include <cmath>
#include "PerlinNoise.h"


PerlinNoise::PerlinNoise() {
	initialize(0);
}

PerlinNoise::PerlinNoise(int seed) {
	initialize(seed);
}

PerlinNoise::~PerlinNoise() {

}

void PerlinNoise::initialize(int seed) {
	this->seed = seed;

	//Create a vector with values from 0 to 255
	permutation = std::vector<int>(256);
	for (int i = 0; i < 256; i++)
		permutation[i] = i;

	//Create a random generator with the seed and shuffle the vector
	std::default_random_engine generator(seed);
	std::shuffle(permutation.begin(), permutation.end(), generator);

	//Duplicate the current order of the vector
	permutation.insert(permutation.end(), permutation.begin(), permutation.end());
}

int PerlinNoise::getSeed() {
	return seed;
}

double PerlinNoise::noise(double x, double y, double z) {
	//Find the unit cube that contains the point
	int X = (int)floor(x) & 255;
	int Y = (int)floor(y) & 255;
	int Z = (int)floor(z) & 255;

	//Find relative x, y, z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	//Compute fade curves for each of x, y, z
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	//Hash the coordinates of the 8 cube corners
	int A = permutation[X] + Y;
	int AA = permutation[A] + Z;
	int AB = permutation[A + 1] + Z;
	int B = permutation[X + 1] + Y;
	int BA = permutation[B] + Z;
	int BB = permutation[B + 1] + Z;

	//Add blended results from 8 corners of cube
	double res = lerp(w, lerp(v, lerp(u, grad(permutation[AA], x, y, z), grad(permutation[BA], x - 1, y, z)), lerp(u, grad(permutation[AB], x, y - 1, z), grad(permutation[BB], x - 1, y - 1, z))), lerp(v, lerp(u, grad(permutation[AA + 1], x, y, z - 1), grad(permutation[BA + 1], x - 1, y, z - 1)), lerp(u, grad(permutation[AB + 1], x, y - 1, z - 1), grad(permutation[BB + 1], x - 1, y - 1, z - 1))));
	return (res + 1.0) / 2.0;
}

double PerlinNoise::fade(double val) {
	return val * val * val * (val * (val * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
	return (1.0 - t) * a + t * b;
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	// Convert lower 4 bits of hash inot 12 gradient directions
	double u = h < 8 ? x : y,
		v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}