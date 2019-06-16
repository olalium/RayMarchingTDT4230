#pragma once

#include "mesh.h"
#include <gloom/imageLoader.hpp>
#include "FastNoise.h"
#include "noiseTexture.hpp"

unsigned int generateBuffer(Mesh &mesh);
unsigned int generateNormalBuffer(Mesh &mesh);
unsigned int generateTextureBuffer(Mesh &mesh);
unsigned int generate3DTextureBuffer(Mesh &mesh);
unsigned int generateNormalTextureBuffer(Mesh &mesh);
unsigned int generateTexture(PNGImage image);
unsigned int generate3DTexture(NoiseTexture noise);