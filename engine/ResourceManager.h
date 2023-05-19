#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>



#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "lodepng.h"

#include "shaderprogram.h"
#include "GameObject.h"


class Model
{
public:
	float* vertices;
	float* texCoords;
	float* normals;
	int vertexCount;
	Model(float* vertices,int vertexCount, float* texCoords);
};

class Texture
{
public:
	GLuint texture;
	GLuint specular;
	int textureUnit;
};

class ResourceManager
{
public:
	ShaderProgram* shader;
	GameObject root;

	std::vector<GameObject*> renderQueue;
	
	std::vector<Model> modelPalette;
	std::vector<Texture> texturePalette;

	ResourceManager();
	void initShaders(const char* vertexSource, const char* fragmentSource);
	void drawScene(glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix);
	int addModel(float* vertices, int vertexCount, float* texCoords = nullptr);
	int addTexture(const char* textureSource, const char* specSource ="");
	GLuint readTexture(const char* filename);
};

