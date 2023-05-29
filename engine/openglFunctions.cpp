#include "openglFunctions.h"

float aspectRatio = 1;

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}


GLFWwindow* initOpenGL(int resX, int resY) {
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);


	if (!glfwInit()) {
		fprintf(stderr, "Nie mo�na zainicjowa� GLFW.\n");
		exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(resX, resY, "OpenGL", NULL, NULL);

	if (!window)
	{
		fprintf(stderr, "Nie mo�na utworzy� okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Nie mo�na zainicjowa� GLEW.\n");
		exit(EXIT_FAILURE);
	}

	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	//callbacks
	

	return window;
}

void freeResources(GLFWwindow* window) {
	glfwDestroyWindow(window); //Usu� kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zaj�te przez GLFW
	exit(EXIT_SUCCESS);
}
