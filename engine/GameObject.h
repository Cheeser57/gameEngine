#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class GameObject
{
public:
	glm::vec3 pos, rot, scale;

	glm::mat4 modelMatrix;
	int modelIndex, textureIndex;
	bool renderFlag, complexModel;

	GameObject* parent;
	std::vector<GameObject*> children;

	//creating new empty object
	GameObject( GameObject* parent = nullptr, // nullptr only for root
				int modelIndex = -1, // -1 -> no model
				int textureIndex = -1,
				glm::vec3 pos = glm::vec3(0, 0, 0),
				glm::vec3 scale = glm::vec3(1, 1, 1),
				glm::vec3 rot = glm::vec3(0, 0, 0),
				bool renderFlag = true,
				bool complexModel = true);

	void updateModelMatrix();
	void updateSelfModelMatrixS();
	void addChild(GameObject* child);
	

};