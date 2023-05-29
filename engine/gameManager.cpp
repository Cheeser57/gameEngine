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

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) myGm->keyStatus[0] = true;
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) myGm->keyStatus[0] = false;
    if (key == GLFW_KEY_S && action == GLFW_PRESS) myGm->keyStatus[1] = true;
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) myGm->keyStatus[1] = false;
    if (key == GLFW_KEY_A && action == GLFW_PRESS) myGm->keyStatus[2] = true;
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) myGm->keyStatus[2] = false;
    if (key == GLFW_KEY_D && action == GLFW_PRESS) myGm->keyStatus[3] = true;
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) myGm->keyStatus[3] = false;
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) myGm->keyStatus[4] = true;
    if (key == GLFW_KEY_Q && action == GLFW_RELEASE) myGm->keyStatus[4] = false;
    if (key == GLFW_KEY_E && action == GLFW_PRESS) myGm->keyStatus[5] = true;
    if (key == GLFW_KEY_E && action == GLFW_RELEASE) myGm->keyStatus[5] = false;
}


GameManager::GameManager(GLFWwindow* window)
{
    myGm = this;
    aspectRatio = 1;
    this->window = window;
    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glfwSetKeyCallback(window, keyCallback);

	status = 1;
    timeDelta = 0;

    position = glm::vec3(0,2,-10);
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
        position,
        glm::vec3(0, 0, 0),
        glm::vec3(0.0f, 1.0f, 0.0f));
}

void GameManager::cameraFollow(glm::vec3 pos)
{
    camera = glm::lookAt(
        position+pos,
        pos,
        glm::vec3(0.0f, 1.0f, 0.0f));
}

void GameManager::updatePerspective()
{
    perspective = glm::perspective(50.0f * 3.14f / 180.0f, aspectRatio, 0.5f, 5000.0f);
}
