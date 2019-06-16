#pragma once
#include "mesh.h"
#include <gloom/imageLoader.hpp>
#include <iostream>

Mesh generateBox(float width, float height, float depth, bool flipFaces = false);
Mesh generateSphere(float radius, int slices, int layers);
Mesh generateGrid(float widthLength, float depthLength, bool flipFaces, float textureGranularity, PNGImage heightMap); // square grid
Mesh generateVolumetricBox(float width, float height, float depth, bool flipFaces = false);