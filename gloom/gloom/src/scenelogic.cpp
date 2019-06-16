#include <chrono>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <gloom/shader.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <gloom/mesh.h>
#include <gloom/shapes.h>
#include <gloom/glutils.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "scenelogic.hpp"
#include "scenenode.hpp"
#include <gloom/imageLoader.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <gloom/noiseTexture.hpp>
#include "gloom/camera.hpp"
#include "gloom/timeutils.h"

float widthLength = 1000.0f;
float depthLength = 700.0f;
Gloom::Shader* shader;
SceneNode* rootNode;
SceneNode* gridNode;
SceneNode* terrainNode;
SceneNode* sunNode;
SceneNode* obsNode;
SceneNode* slSun;
SceneNode* slObs;
SceneNode* tex3DNode;
Gloom::Camera* cam = new Gloom::Camera();

FastNoise Perlin;
NoiseTexture perlinTexture;

void initScene(GLFWwindow* window) {
	shader = new Gloom::Shader();
    shader->makeBasicShader("../gloom/res/shaders/simple.vert", "../gloom/res/shaders/simple.frag");
    shader->activate();

    std::string heigthMapLocation = "../gloom/res/textures/Heightmap.png";
    PNGImage heigthMapImage = loadPNGFile(heigthMapLocation);

    // terrainGrid
    std::string grassLocation = "../gloom/res/textures/grass.png";
    PNGImage grassImage = loadPNGFile(grassLocation);
    Mesh terrainGrid = generateGrid(widthLength, depthLength, false, 20, heigthMapImage);
    
    unsigned int tGridVAO = generateTextureBuffer(terrainGrid);
    unsigned int tTexVao = generateTexture(grassImage);
    
    terrainNode = createSceneNode();
    terrainNode->position = glm::vec3(-(widthLength/2), -20, 0.0f);
    terrainNode->rotation = glm::vec3(3.14,0,0);
    terrainNode->vertexArrayObjectID = tGridVAO;
    terrainNode->texID = tTexVao;
    terrainNode->VAOIndexCount = terrainGrid.indices.size();

    // sun light
    Mesh sphere = generateSphere(5.0, 40, 40);
    unsigned int sunVAO = generateNormalBuffer(sphere);
    slSun = createSceneNode();
    sunNode = createSceneNode();
    sunNode->vertexArrayObjectID = sunVAO;
    sunNode->VAOIndexCount = sphere.indices.size();
    sunNode->position = glm::vec3(-100, 200, -200);
    slSun->nodeType = POINT_LIGHT;
    slSun->lightID = 0;

    // 3D Perlin noise texture..
    Mesh volumeBox = generateVolumetricBox(1000, 10, 700, false);
    unsigned int volumeBoxVAO = generate3DTextureBuffer(volumeBox);

    Perlin.SetNoiseType(FastNoise::PerlinFractal);
    Perlin.SetFrequency(0.057f);
    perlinTexture = generateNoiseTexture(Perlin, 300, 100, 300);
    unsigned int tex3DID = generate3DTexture(perlinTexture);
    tex3DNode = createSceneNode();
    tex3DNode->nodeType = THREE_D_GEO;
    tex3DNode->vertexArrayObjectID = volumeBoxVAO;
    tex3DNode->VAOIndexCount = volumeBox.indices.size();
    tex3DNode->texID = tex3DID;
    tex3DNode->position = glm::vec3(-500, 100, -700);
    tex3DNode->rotation = glm::vec3(3.14,3.14,3.14);

    // root 
    rootNode = createSceneNode();
    rootNode->children.push_back(sunNode);
    sunNode->children.push_back(slSun);
    rootNode->children.push_back(terrainNode);
    rootNode->children.push_back(tex3DNode);
}

void updateNodeTransformations(SceneNode* node, glm::mat4 transformationThusFar, glm::mat4 viewTransformation) {

    glm::mat4 transformationMatrix(1.0);

    switch(node->nodeType) {
        case GEOMETRY:
            transformationMatrix =
                    glm::translate(glm::mat4(1.0), node->position)
                    * glm::translate(glm::mat4(1.0), node->referencePoint)
                    * glm::rotate(glm::mat4(1.0), node->rotation.z, glm::vec3(0,0,1))
                    * glm::rotate(glm::mat4(1.0), node->rotation.y, glm::vec3(0,1,0))
                    * glm::rotate(glm::mat4(1.0), node->rotation.x, glm::vec3(1,0,0))
                    * glm::translate(glm::mat4(1.0), -node->referencePoint)
                    * glm::scale(glm::mat4(1.0), node->scale);
            break;
        case POINT_LIGHT:
            break;
        case SPOT_LIGHT:
        {
            break;
        }
        case TWO_D_GEO:
        {
            break;
        }
        case NORMAL_GEO:
        {
            break;
        }
        case THREE_D_GEO:
        {
            transformationMatrix =
                    glm::translate(glm::mat4(1.0), node->position)
                    * glm::translate(glm::mat4(1.0), node->referencePoint)
                    * glm::rotate(glm::mat4(1.0), node->rotation.z, glm::vec3(0,0,1))
                    * glm::rotate(glm::mat4(1.0), node->rotation.y, glm::vec3(0,1,0))
                    * glm::rotate(glm::mat4(1.0), node->rotation.x, glm::vec3(1,0,0))
                    * glm::translate(glm::mat4(1.0), -node->referencePoint)
                    * glm::scale(glm::mat4(1.0), node->scale);

        }
    }

    node->MV = viewTransformation * transformationMatrix;
    node->currentTransformationMatrix = transformationThusFar * transformationMatrix;

    for(SceneNode* child : node->children) {
        updateNodeTransformations(child, node->currentTransformationMatrix, viewTransformation);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    cam->handleKeyboardInputs(key, action);
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    cam->handleCursorPosInput(xpos, ypos);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    cam->handleMouseButtonInputs(button, action);
}

void updateFrame(GLFWwindow* window) {
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    double timeDelta = getTimeDeltaSeconds();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(windowWidth) / float(windowHeight), 0.1f,
                                            750.0f);

    cam->updateCamera(timeDelta);
    glm::mat4 cameraTransform = cam->getViewMatrix();

    glm::mat4 VP = projection * cameraTransform;

    updateNodeTransformations(rootNode, VP, cameraTransform);

    slSun->position = sunNode->position;
}
void renderNode(SceneNode* node) {
	glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(node->currentTransformationMatrix));
	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(node->MV));

    // set default is not 3D texture
    glUniform1i(7, 0);

    switch(node->nodeType) {
        case GEOMETRY:
            glBindTextureUnit(0, node->texID);
            if(node->vertexArrayObjectID != -1) {
                glBindVertexArray(node->vertexArrayObjectID);
                glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, nullptr);
            }
            break;
        case POINT_LIGHT:
        {
            glm::vec3 light_position = glm::vec3(node->MV * glm::vec4(node->position, 1.0f));
            switch(node->lightID) {
                case 0:
                {
                    glUniform3f(9, light_position[0], light_position[1], light_position[2]);
                    break;
                }
                case 1:
                {
                    glUniform3f(10, light_position[0], light_position[1], light_position[2]);
                    break;
                }
            }
            if(node->vertexArrayObjectID != -1) {
                glBindVertexArray(node->vertexArrayObjectID);
                glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, nullptr);
            }
            break;
        }
        case SPOT_LIGHT:
        {
            break;
        }
        case TWO_D_GEO:
        {
            break;
        }
        case NORMAL_GEO:
        {
            break;
        }
        case THREE_D_GEO:
        {   
            glUniform1i(7, 1); // is 3D texture
            glBindTextureUnit(1, node->texID);

            if(node->vertexArrayObjectID != -1) {
                glBindVertexArray(node->vertexArrayObjectID);
                glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, nullptr);
            }
            break;
        }
    }

	for(SceneNode* child : node->children) {
        renderNode(child);
    }
}
void renderFrame(GLFWwindow* window) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
    renderNode(rootNode);
}