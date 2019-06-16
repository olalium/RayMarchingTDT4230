#include "noiseTexture.hpp"
#include <iostream>

NoiseTexture generateNoiseTexture(FastNoise noise, unsigned int width, unsigned int height, unsigned int depth) {
	std::vector<float> voxels; //in form RGBA
	float alpha; // transparency level between 0 and 1

	// i think this is necessary to make the data open_gl friendly..
	for(unsigned int z = 0; z < depth; z++) {
		for(unsigned int y = 0; y < height; y++) {
			for(unsigned int x = 0; x < width; x++) {
				alpha = (1 + noise.GetPerlin(x,y,z)) / 2;
				voxels.push_back(1.0f); // R
				voxels.push_back(1.0f); // G
				voxels.push_back(1.0f); // B
				voxels.push_back(alpha);// A
			}
		}
	}

	NoiseTexture texture;
	texture.width = width;
	texture.height = height;
	texture.depth = depth;
	texture.voxels = voxels;

	return texture;
}