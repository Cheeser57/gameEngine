#pragma once
//Root Object Menager

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <stdlib.h>
#include <stdio.h>

#include "openglFunctions.h"
#include "gameObject.h"



class GameManager
{
public:
	GLFWwindow* window;
	int status;

	float aspectRatio;
	float timeDelta;
	glm::mat4 camera;
	glm::mat4 perspective;

	GameObject cameraObj;
	void updateTimeDelta();
	void updateCamera();
	void updatePerspective();

	GameManager(GLFWwindow* window);
};

