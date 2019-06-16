#include <iostream>
#include "shapes.h"
#include <functional>
#include <numeric>
#include <vector>
#include <iterator>
#include <algorithm>

Mesh generateBox(float width, float height, float depth, bool flipFaces) {
    // Hardcoded. Sue me.

    // Edit: well, that backfired..

    std::vector<glm::vec3> vertices = {
            {0,     0,      0},
            {0,     0,      depth},
            {0,     height, depth},

            {0,     0,      0},
            {0,     height, depth},
            {0,     height, 0},

            {width, 0,      0},
            {width, height, depth},
            {width, 0,      depth},

            {width, 0,      0},
            {width, height, 0},
            {width, height, depth},

            {0,     0,      0},
            {width, height, 0},
            {width, 0,      0},

            {0,     0,      0},
            {0,     height, 0},
            {width, height, 0},

            {0,     0,      depth},
            {width, 0,      depth},
            {width, height, depth},

            {0,     0,      depth},
            {width, height, depth},
            {0,     height, depth},

            {0,     0,      0},
            {width, 0,      0},
            {width, 0,      depth},

            {0,     0,      0},
            {width, 0,      depth},
            {0,     0,      depth},

            {width, height, 0},
            {0,     height, 0},
            {0,     height, depth},

            {width, height, 0},
            {0,     height, depth},
            {width, height, depth},
    };
    std::vector<glm::vec3> normals = {
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0}
    };

    float texScaleFactorX = depth / height;
    float texScaleFactorY = width / depth;
    float texScaleFactorZ = width / height;
    std::cout << "texScaleFactorX:\t" << texScaleFactorX << "\n";
    std::cout << "texScaleFactorY:\t" << texScaleFactorY << "\n";
    std::cout << "texScaleFactorZ:\t" << texScaleFactorZ << "\n";
    
    std::vector<glm::vec2> textureCoordinates = {
            {0, 0},
            {texScaleFactorX, 0},
            {texScaleFactorX, 1},

            {0, 0},
            {texScaleFactorX, 1},
            {0, 1},

            {0, 0},
            {texScaleFactorX, 1},
            {texScaleFactorX, 0},

            {0, 0},
            {0, 1},
            {texScaleFactorX, 1},

            {0, 0},
            {texScaleFactorZ, 0},
            {texScaleFactorZ, 1},

            {0, 0},
            {texScaleFactorZ, 1},
            {0, 1},

            {0, 0},
            {texScaleFactorZ, 0},
            {texScaleFactorZ, 1},

            {0, 0},
            {texScaleFactorZ, 1},
            {0, 1},

            {0, 0},
            {texScaleFactorY, 0},
            {texScaleFactorY, 1},

            {0, 0},
            {texScaleFactorY, 1},
            {0, 1},

            {0, 0},
            {texScaleFactorY, 0},
            {texScaleFactorY, 1},

            {0, 0},
            {texScaleFactorY, 1},
            {0, 1},
    };


    std::vector<unsigned int> indices = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11,
            12, 13, 14,
            15, 16, 17,
            18, 19, 20,
            21, 22, 23,
            24, 25, 26,
            27, 28, 29,
            30, 31, 32,
            33, 34, 35
    };

    if(flipFaces) {
        for(int i = 0; i < 36; i += 3) {
            unsigned int temp = indices[i + 1];
            indices[i + 1] = indices[i + 2];
            indices[i + 2] = temp;

            normals[i + 0] *= -1;
            normals[i + 1] *= -1;
            normals[i + 2] *= -1;
        }
    }

    Mesh mesh;
    mesh.vertices = vertices;
    mesh.normals = normals;
    mesh.textureCoordinates = textureCoordinates;
    mesh.indices = indices;

    return mesh;
}


Mesh generateSphere(float sphereRadius, int slices, int layers) {
    const unsigned int triangleCount = slices * layers * 2;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    vertices.reserve(3 * triangleCount);
    normals.reserve(3 * triangleCount);
    indices.reserve(3 * triangleCount);

    // Slices require us to define a full revolution worth of triangles.
    // Layers only requires angle varying between the bottom and the top (a layer only covers half a circle worth of angles)
    const float degreesPerLayer = 180.0 / (float) layers;
    const float degreesPerSlice = 360.0 / (float) slices;

    unsigned int i = 0;

    // Constructing the sphere one layer at a time
    for (int layer = 0; layer < layers; layer++) {
        int nextLayer = layer + 1;

        // Angles between the vector pointing to any point on a particular layer and the negative z-axis
        float currentAngleZDegrees = degreesPerLayer * layer;
        float nextAngleZDegrees = degreesPerLayer * nextLayer;

        // All coordinates within a single layer share z-coordinates.
        // So we can calculate those of the current and subsequent layer here.
        float currentZ = -cos(glm::radians(currentAngleZDegrees));
        float nextZ = -cos(glm::radians(nextAngleZDegrees));

        // The row of vertices forms a circle around the vertical diagonal (z-axis) of the sphere.
        // These radii are also constant for an entire layer, so we can precalculate them.
        float radius = sin(glm::radians(currentAngleZDegrees));
        float nextRadius = sin(glm::radians(nextAngleZDegrees));

        // Now we can move on to constructing individual slices within a layer
        for (int slice = 0; slice < slices; slice++) {

            // The direction of the start and the end of the slice in the xy-plane
            float currentSliceAngleDegrees = slice * degreesPerSlice;
            float nextSliceAngleDegrees = (slice + 1) * degreesPerSlice;

            // Determining the direction vector for both the start and end of the slice
            float currentDirectionX = cos(glm::radians(currentSliceAngleDegrees));
            float currentDirectionY = sin(glm::radians(currentSliceAngleDegrees));

            float nextDirectionX = cos(glm::radians(nextSliceAngleDegrees));
            float nextDirectionY = sin(glm::radians(nextSliceAngleDegrees));

            vertices.emplace_back(sphereRadius * radius * currentDirectionX,
                                  sphereRadius * radius * currentDirectionY,
                                  sphereRadius * currentZ);
            vertices.emplace_back(sphereRadius * radius * nextDirectionX,
                                  sphereRadius * radius * nextDirectionY,
                                  sphereRadius * currentZ);
            vertices.emplace_back(sphereRadius * nextRadius * nextDirectionX,
                                  sphereRadius * nextRadius * nextDirectionY,
                                  sphereRadius * nextZ);
            vertices.emplace_back(sphereRadius * radius * currentDirectionX,
                                  sphereRadius * radius * currentDirectionY,
                                  sphereRadius * currentZ);
            vertices.emplace_back(sphereRadius * nextRadius * nextDirectionX,
                                  sphereRadius * nextRadius * nextDirectionY,
                                  sphereRadius * nextZ);
            vertices.emplace_back(sphereRadius * nextRadius * currentDirectionX,
                                  sphereRadius * nextRadius * currentDirectionY,
                                  sphereRadius * nextZ);

            normals.emplace_back(radius * currentDirectionX,
                                 radius * currentDirectionY,
                                 currentZ);
            normals.emplace_back(radius * nextDirectionX,
                                 radius * nextDirectionY,
                                 currentZ);
            normals.emplace_back(nextRadius * nextDirectionX,
                                 nextRadius * nextDirectionY,
                                 nextZ);
            normals.emplace_back(radius * currentDirectionX,
                                 radius * currentDirectionY,
                                 currentZ);
            normals.emplace_back(nextRadius * nextDirectionX,
                                 nextRadius * nextDirectionY,
                                 nextZ);
            normals.emplace_back(nextRadius * currentDirectionX,
                                 nextRadius * currentDirectionY,
                                 nextZ);

            indices.emplace_back(i + 0);
            indices.emplace_back(i + 1);
            indices.emplace_back(i + 2);
            indices.emplace_back(i + 3);
            indices.emplace_back(i + 4);
            indices.emplace_back(i + 5);

            i += 6;
        }
    }

    Mesh mesh;
    mesh.vertices = vertices;
    mesh.normals = normals;
    mesh.indices = indices;
    return mesh;
}

float f(float x, float y, float height) {
    return height/7.5; //sin(x*2.0f*3.141526f) * sin(y*2.0f*3.141526f) * 0.05f;
}

Mesh generateGrid(float widthLength, float depthLength, bool flipFaces, float textureGranularity, PNGImage heightMap) {
    Mesh mesh;
    glm::vec3 p1, p2, p3, v1, v2, nv; //nv = normal vector
    glm::vec2 uv1, uv2, uv3;

    float gridSize = heightMap.width;
    int index = 0;
    int pixelChannels = 4; // RGBA
    float x, y, z, ux, uy, height;

    float wpg = widthLength / (float)gridSize; //width per grid
    float dpg = depthLength / (float)gridSize; //depth per grid
    float wpgow = wpg / textureGranularity; // wpg over texture granularity
    float dpgod = dpg / textureGranularity;// dpg over texture granularity
    float ppgWidth= (float)heightMap.width / (float)gridSize; // pixels per grid, width
    float ppgDepth= (float)heightMap.height / (float)gridSize; // pixels per grid, depth

    unsigned int
        offset,
        rowIndex1,
        rowIndex2,
        startPixelIndexUpperRight,
        startPixelIndexUpperLeft,
        startPixelIndexLowerRight,
        startPixelIndexLowerLeft,
        pixelIndexUpperRight,
        pixelIndexUpperLeft,
        pixelIndexLowerRight,
        pixelIndexLowerLeft;

    for (int i = 0; i < gridSize; i++) {
        rowIndex1 = (unsigned int) i * ppgDepth;
        rowIndex2 = (unsigned int) (i + 1) * ppgDepth;
        if ( rowIndex2 > (heightMap.height - 1) ) {
            rowIndex2 = heightMap.height - ppgDepth;
            if ( rowIndex1 > (heightMap.height - 1)) {
                rowIndex1 = heightMap.height - 2 * ppgDepth;
            }
        }
        if (i == 0) {
            startPixelIndexUpperLeft = 0;
            startPixelIndexUpperRight = (unsigned int) ppgWidth;;
            startPixelIndexLowerLeft = heightMap.width - 1;
            startPixelIndexLowerRight = startPixelIndexLowerLeft + (unsigned int) ppgWidth;
        } else {
            startPixelIndexUpperLeft = rowIndex2 * heightMap.width - 1;
            startPixelIndexUpperRight = startPixelIndexUpperLeft + (unsigned int) ppgWidth;
            startPixelIndexLowerLeft = rowIndex1 * heightMap.width - 1;
            startPixelIndexLowerRight = startPixelIndexLowerLeft + (unsigned int) ppgWidth;
        }
    
        int ex = 0;
        for (int j = 0; j < gridSize; j++) {
            offset = (unsigned int) (j * pixelChannels * ppgWidth);
            pixelIndexUpperLeft = startPixelIndexUpperLeft * pixelChannels + offset;
            pixelIndexLowerLeft = startPixelIndexLowerLeft * pixelChannels + offset;
            pixelIndexUpperRight = startPixelIndexUpperRight * pixelChannels + offset;
            pixelIndexLowerRight = startPixelIndexLowerRight * pixelChannels + offset;

            //
            // triangle 1
            //
            // lower left
            height = heightMap.pixels.at(pixelIndexLowerLeft);
            x = j*wpg;
            z = i*dpg;
            ux = x / textureGranularity;
            uy = z / textureGranularity;
            y = f(x, z, height);
            p1 = glm::vec3(x, y, z);
            uv1 = glm::vec2(ux, uy);
            mesh.vertices.push_back(p1);
            mesh.textureCoordinates.push_back(uv1);

            // lower right
            height = heightMap.pixels.at(pixelIndexLowerRight);
            x += wpg;
            ux += wpgow;
            y = f(x, z, height);
            p2 = glm::vec3(x, y, z);
            uv2 = glm::vec2(ux, uy);
            mesh.vertices.push_back(p2);
            mesh.textureCoordinates.push_back(uv2);

            // upper left
            height = heightMap.pixels.at(pixelIndexUpperLeft);
            x -= wpg;
            z += dpg;
            ux -= wpgow;
            uy += dpgod;
            y = f(x, z, height);
            p3 = glm::vec3(x, y, z);
            uv3 = glm::vec2(ux, uy);
            mesh.vertices.push_back(p3);
            mesh.textureCoordinates.push_back(uv3);

            //calculate and push normals
            v1 = p2 - p1;
            v2 = p3 - p2;

            nv.x = v1.y * v2.z - v1.z * v2.y;
            nv.y = v1.z * v2.x - v1.x * v2.z;
            nv.z = v1.x * v2.y - v1.y * v2.x;
            mesh.normals.push_back(nv);
            mesh.normals.push_back(nv);
            mesh.normals.push_back(nv);

            //
            // triangle 2
            //
            // lower right
            height = heightMap.pixels.at(pixelIndexLowerRight);
            x += wpg;
            z -= dpg;
            ux += wpgow;
            uy -= dpgod;
            y = f(x, z, height);
            p1 = glm::vec3(x, y, z);
            uv1 = glm::vec2(ux, uy);
            mesh.vertices.push_back(p1);
            mesh.textureCoordinates.push_back(uv1);

            // upper right
            height = heightMap.pixels.at(pixelIndexUpperRight);
            z += dpg;
            uy += dpgod;
            y = f(x, z, height);
            p2 = glm::vec3(x, y, z);
            uv2 = glm::vec2(ux, uy);
            mesh.vertices.push_back(p2);
            mesh.textureCoordinates.push_back(uv2);

            // upper left
            height = heightMap.pixels.at(pixelIndexUpperLeft);
            x -= wpg;
            ux -= wpgow;
            y = f(x, z, height);
            p3 = glm::vec3(x, y, z);
            uv3 = glm::vec2(ux, uy);
            mesh.vertices.push_back(p3);
            mesh.textureCoordinates.push_back(uv3);

            //calculate and push normals
            v1 = p2 - p1;
            v2 = p3 - p2;

            nv.x = v1.y * v2.z - v1.z * v2.y;
            nv.y = v1.z * v2.x - v1.x * v2.z;
            nv.z = v1.x * v2.y - v1.y * v2.x;
            mesh.normals.push_back(nv);
            mesh.normals.push_back(nv);
            mesh.normals.push_back(nv);

            //update indices for both triangles
            for (int i = 0; i <= 5; i++) {
                mesh.indices.push_back(index++);
            }
        }
    }
    return mesh;
}

Mesh generateVolumetricBox(float width, float height, float depth, bool flipFaces) {

    std::vector<glm::vec3> vertices = {
            {0,     0,      0},
            {0,     0,      depth},
            {0,     height, depth},

            {0,     0,      0},
            {0,     height, depth},
            {0,     height, 0},

            {width, 0,      0},
            {width, height, depth},
            {width, 0,      depth},

            {width, 0,      0},
            {width, height, 0},
            {width, height, depth},

            {0,     0,      0},
            {width, height, 0},
            {width, 0,      0},

            {0,     0,      0},
            {0,     height, 0},
            {width, height, 0},

            {0,     0,      depth},
            {width, 0,      depth},
            {width, height, depth},

            {0,     0,      depth},
            {width, height, depth},
            {0,     height, depth},

            {0,     0,      0},
            {width, 0,      0},
            {width, 0,      depth},

            {0,     0,      0},
            {width, 0,      depth},
            {0,     0,      depth},

            {width, height, 0},
            {0,     height, 0},
            {0,     height, depth},

            {width, height, 0},
            {0,     height, depth},
            {width, height, depth},
    };

    std::vector<glm::vec3> normals = {
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},

            {1.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},

            {1.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},
            {1.0, 0.0, 0.0},

            {0.0, 0.0, -1.0},
            {0.0, 0.0, -1.0},
            {0.0, 0.0, -1.0},

            {0.0, 0.0, -1.0},
            {0.0, 0.0, -1.0},
            {0.0, 0.0, -1.0},

            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},

            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},

            {0.0, -1.0, 0.0},
            {0.0, -1.0, 0.0},
            {0.0, -1.0, 0.0},

            {0.0, -1.0, 0.0},
            {0.0, -1.0, 0.0},
            {0.0, -1.0, 0.0},

            {0.0, 1.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 1.0, 0.0},

            {0.0, 1.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 1.0, 0.0}
    };
    
    std::vector<glm::vec3> textureCoordinates = {
            {0, 0, 0},
            {0, 0, 1},
            {0, 1, 1},

            {0, 0, 0},
            {0, 1, 1},
            {0, 1, 0},

            {1, 0, 0},
            {1, 1, 1},
            {1, 0, 1},

            {1, 0, 0},
            {1, 1, 0},
            {1, 1, 1},

            {0, 0, 0},
            {1, 1, 0},
            {1, 0, 0},

            {0, 0, 0},
            {0, 1, 0},
            {1, 1, 0},

            {0, 0, 1},
            {1, 0, 1},
            {1, 1, 1},

            {0, 0, 1},
            {1, 1, 1},
            {0, 1, 1},

            {0, 0, 0},
            {1, 0, 0},
            {1, 0, 1},

            {0, 0, 0},
            {1, 0, 1},
            {0, 0, 1},

            {1, 1, 0},
            {0, 1, 0},
            {0, 1, 1},

            {1, 1, 0},
            {0, 1, 1},
            {1, 1, 1}
    };


    std::vector<unsigned int> indices = {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8,
            9, 10, 11,
            12, 13, 14,
            15, 16, 17,
            18, 19, 20,
            21, 22, 23,
            24, 25, 26,
            27, 28, 29,
            30, 31, 32,
            33, 34, 35
    };

    if(flipFaces) {
        for(int i = 0; i < 36; i += 3) {
            unsigned int temp = indices[i + 1];
            indices[i + 1] = indices[i + 2];
            indices[i + 2] = temp;

            normals[i + 0] *= -1;
            normals[i + 1] *= -1;
            normals[i + 2] *= -1;
        }
    }

    Mesh mesh;
    mesh.vertices = vertices;
    mesh.normals = normals;
    mesh.textureCoordinates3D = textureCoordinates;
    mesh.indices = indices;

    return mesh;
}