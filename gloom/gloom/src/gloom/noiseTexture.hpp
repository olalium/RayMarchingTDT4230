#pragma once

#include <vector>
#include "FastNoise.h"

typedef struct NoiseTexture {
	float width, height, depth;
	std::vector<float> voxels;
} NoiseTexture;

NoiseTexture generateNoiseTexture(FastNoise noise, unsigned int width, unsigned int height, unsigned int depth);