#include "gameManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GameManager* myGm;

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    
    if (height == 0) return;
    myGm->aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
    myGm->updatePerspective();
    printf("resize!\n");
}


GameManager::GameManager(GLFWwindow* window)
{
    myGm = this;
    aspectRatio = 1;
    this->window = window;
    glfwSetWindowSizeCallback(window, windowResizeCallback);

	status = 1;
    timeDelta = 0;

    cameraObj = GameObject(nullptr,-1,-1,glm::vec3(0,2,-10));
    updatePerspective();
    updateCamera();
}

void GameManager::updateTimeDelta()
{
    timeDelta = glfwGetTime();
    glfwSetTime(0);
}

void GameManager::updateCamera()
{
    camera = glm::lookAt(
        cameraObj.pos,
        glm::vec3(0, 0, 0),
        glm::vec3(0.0f, 1.0f, 0.0f));
}


void GameManager::updatePerspective()
{
    perspective = glm::perspective(50.0f * 3.14f / 180.0f, aspectRatio, 0.01f, 50.0f);
}
