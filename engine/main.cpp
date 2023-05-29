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
#include "PhysObject.h"

#include "myCube.h"
#include "myTeapot.h"
#include "flatPlane.h"


#define PI 3.1413

int main()
{
    GameManager gm(initOpenGL(1024, 1024));
    ResourceManager res;

    res.initShaders("vertex.glsl","vertex2.glsl", "fragment.glsl");

///To be moved into other file
    //Model definitions
    int cubeModel = res.addModel(myCubeVertices, myCubeVertexCount,myCubeTexCoords);
    //int potModel = res.addModel(myTeapotVertices, myTeapotVertexCount,myTeapotTexCoords);
    int flatPlane = res.addModel(flatPlaneVertecies, flatPlaneCount, flatPlaneTexCoords);

    int planeModel = res.loadModel("piper.obj");

    //Texture definitions
    int tex1 = res.addTexture("metal.png");
    int tex2 = res.addTexture("tarmac.png");
    int tex3 = res.addTexture("piper_diffuse.png");

    //Object definitions
    GameObject plane = PhysObject(&res.root, planeModel,tex3,glm::vec3(0,0,8), glm::vec3(1), glm::vec3(0,-PI/2,0));
    plane.updateModelMatrix();
    //plane.complexModel = false;
    res.renderQueue2.push_back(&plane);

    GameObject box = GameObject(&plane, cubeModel, tex1, glm::vec3(2, 0, 0));
    box.scale *= 0.5;
    box.complexModel = false;
    res.renderQueue.push_back(&box);

    GameObject floor = GameObject(&res.root, flatPlane, tex2);
    floor.scale *= 250;
    //floor.complexModel = false;
    floor.updateModelMatrix();
    res.renderQueue.push_back(&floor);
///
    glfwSetTime(0);
    while(!glfwWindowShouldClose(gm.window) && gm.status != 0)
    {
        gm.updateTimeDelta();
        //printf("%f\n", 1 / gm.timeDelta);

        //Game logic - will be moved into function / other file
        //plane.updatePhysics(gm.timeDelta);
        //plane.checkCollision(&floor);
        plane.updateModelMatrix();

        gm.cameraFollow(plane.pos);

        res.drawScene(gm.camera,gm.perspective);
        glfwSwapBuffers(gm.window);

        glfwPollEvents();
        if (gm.keyStatus[0]) plane.rot.z -= gm.timeDelta;
        if (gm.keyStatus[1]) plane.rot.z += gm.timeDelta;
        if (gm.keyStatus[2]) plane.rot.x += gm.timeDelta;
        if (gm.keyStatus[3]) plane.rot.x -= gm.timeDelta;
        if (gm.keyStatus[4]) plane.rot.y += gm.timeDelta;
        if (gm.keyStatus[5]) plane.rot.y -= gm.timeDelta;
    }

    freeResources(gm.window);
}

