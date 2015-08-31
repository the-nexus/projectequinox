#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <vector>

class PerlinNoise {
public:
	PerlinNoise();
	PerlinNoise(int seed);
	~PerlinNoise();
	double noise(double x, double y, double z);
private:
	void initialize(int seed);
	double fade(double val);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);


	int seed;
	std::vector<int> permutation;
};

#endif