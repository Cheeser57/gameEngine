#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "lodepng.h"
#include <fstream>

#include "shaderprogram.h"
#include "GameObject.h"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO, VBO, EBO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void setupMesh();
};

class Model
{
public:
	std::vector<Mesh> meshes;
	void draw();
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};

class simpleModel
{
public:
	float* vertices;
	float* texCoords;

	int vertexCount;
	simpleModel(float* vertices,int vertexCount, float* texCoords);
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
	ShaderProgram* complexShader;
	GameObject root;

	std::vector<GameObject*> renderQueue;
	std::vector<GameObject*> renderQueue2;
	
	std::vector<simpleModel> modelPalette;
	std::vector<Model> modelPalette2;
	std::vector<Texture> texturePalette;

	ResourceManager();
	void initShaders(const char* vertexSource, const char* vertexSource2, const char* fragmentSource);
	void drawScene(glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix);
	int addModel(float* vertices, int vertexCount, float* texCoords = nullptr);
	int loadModel(const char* fileName); //loading model from .obj file
	int addTexture(const char* textureSource, const char* specSource ="");
	GLuint readTexture(const char* filename);
};

