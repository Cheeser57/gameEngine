#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "openglFunctions.h"
#include "gameManager.h"
#include "ResourceManager.h"
#include "GameObject.h"

#include "myCube.h"
#include "myTeapot.h"
#include "flatPlane.h"


int main()
{
    GameManager gm(initOpenGL(512, 512));
    ResourceManager res;

    res.initShaders("vertex.glsl", "fragment.glsl");

///To be moved into other file
    //Model definitions
    int cubeModel = res.addModel(myCubeVertices, myCubeVertexCount,myCubeTexCoords);
    int potModel = res.addModel(myTeapotVertices, myTeapotVertexCount,myTeapotTexCoords);
    int flatPlane = res.addModel(flatPlaneVertecies, flatPlaneCount, flatPlaneTexCoords);

    //Texture definitions
    int tex1 = res.addTexture("metal.png");
    int tex2 = res.addTexture("tarmac.png");

    //Object definitions
    GameObject pot = GameObject(&res.root, potModel,tex1);
    pot.scale *= 2;
    pot.pos.y = 1;
    res.renderQueue.push_back(&pot);

    GameObject pot2 = GameObject(&pot, cubeModel,tex1);
    pot2.scale *= 0.5f;
    pot2.pos.x = -3;
    res.renderQueue.push_back(&pot2);

    GameObject floor = GameObject(&res.root, flatPlane, tex2);
    floor.scale *= 20;
    floor.updateModelMatrix();
    res.renderQueue.push_back(&floor);
///

    glfwSetTime(0);
    while(!glfwWindowShouldClose(gm.window) && gm.status != 0)
    {
        gm.updateTimeDelta();
        //printf("%f\n", 1 / gm.timeDelta);

        //Game logic - will be moved into function / other file
        pot.pos.x += gm.timeDelta*0.7f;
        if (pot.pos.x > 10) pot.pos.x = -5;
        pot.updateModelMatrix();

        res.drawScene(gm.camera,gm.perspective);
        glfwSwapBuffers(gm.window);

        glfwPollEvents();
    }

    freeResources(gm.window);
}

