#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

class PerlinNoise {
public:
	static void generateNoise1D(double* data, int size, double persistence);
private:
	static double* generateOctave(double size, double frequency, double amplitude);
};

#endif