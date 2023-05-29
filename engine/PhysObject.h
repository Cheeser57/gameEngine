#pragma once
#include "GameObject.h"
#include <iostream>
#include <cmath>

struct constants
{
	glm::vec3 g = glm::vec3(0,-9.81,0);
	glm::mat3 inertiaTensor = glm::mat3(glm::vec3(1200, 0, -800), glm::vec3(0, 3500, 0), glm::vec3(-800, 0, 2500));
	float pi = 3.1413f;
	float halfRo = 0.6f;
};

float liftCoef(glm::vec3 airstream, glm::vec3 forawrd);
float sumOfVec3(glm::vec3 vec);



class PhysObject : public GameObject
{
public:
	glm::vec3 vel;
	glm::vec3 angularVel;
	float mass;
	glm::vec3 dragCross;
	float thrust;
	float wingArea;

	glm::vec3 centerOfMass;
	glm::vec3 centerOfLift;
	glm::vec3 centerOfThrust;
	bool grounded = false;

	float boundingBoxSize;

	void updatePhysics(float timeDelta);
	void checkCollision(GameObject* collider);

	PhysObject(GameObject* parent = nullptr, // nullptr only for root
		int modelIndex = -1, // -1 -> no model
		int textureIndex = -1,
		glm::vec3 pos = glm::vec3(0, 0, 0),
		glm::vec3 scale = glm::vec3(1, 1, 1),
		glm::vec3 rot = glm::vec3(0, 1, 0),
		bool renderFlag = true,
		bool ccomplexModel = true);
};

