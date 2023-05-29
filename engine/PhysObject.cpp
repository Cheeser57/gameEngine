#include "PhysObject.h"

constants cons;

PhysObject::PhysObject(GameObject* parent,
	int modelIndex,	// -1 -> no model
	int textureIndex,
	glm::vec3 pos,
	glm::vec3 scale,
	glm::vec3 rot,
	bool renderFlag,
	bool complexModel)
{

	this->pos = pos;
	this->rot = rot;
	this->scale = scale;

	this->vel = glm::vec3(0);
	this->angularVel = glm::vec3(0);
	this->mass = 2000.0f;
	this->dragCross = glm::vec3(0.04f*5,0.2f*20,0.15f*10);
	this->thrust = 2500;
	this->wingArea = 25.0f;

	this->modelIndex = modelIndex;
	this->textureIndex = textureIndex;

	this->parent = parent;
	if (parent != nullptr) parent->addChild(this);

	if (modelIndex != -1)
	{
		this->renderFlag = renderFlag;
		this->complexModel = complexModel;
		updateModelMatrix();
	}
	else this->renderFlag = false;

	boundingBoxSize = scale.y;
}

void PhysObject::updatePhysics(float timeDelta)
{
	//THRUST*ROT_forward - DRAG*ROT_forward + LIFT*ROT_up - GRAVITY
	glm::vec3 forward = glm::vec3(glm::cos(rot.y)*glm::cos(rot.z), glm::sin(rot.z),- glm::sin(rot.y) * glm::cos(rot.z));
	glm::vec3 wing = glm::vec3(glm::cos(rot.y+cons.pi/2) * glm::cos(rot.x), glm::sin(rot.x), -glm::sin(rot.y + cons.pi / 2) * glm::cos(rot.x));

	glm::vec3 velUp = glm::normalize(glm::cross(vel, wing));

	//glm::vec3 forwardVec = glm::normalize(glm::vec3(forward * glm::vec4(1, 0, 0, 0)));
	//glm::vec3 upVec = glm::normalize(glm::vec3(up * glm::vec4(1, 0, 0, 0)));

	glm::vec3 thrustVec = forward * thrust;
	glm::vec3 drag = -(abs(vel) * vel) * cons.halfRo * dragCross;
	
	std::cout << "Forward: " << forward.x << ' ' << forward.y << ' ' << forward.z << std::endl;
	std::cout << "Wing: " << wing.x << ' ' << wing.y << ' ' << wing.z << std::endl;
	std::cout << "Up: " << velUp.x << ' ' << velUp.y << ' ' << velUp.z << std::endl;
	std::cout << "Vel:" << vel.x << ' ' << vel.y << ' ' << vel.z << std::endl;
	std::cout << "Thr:" << thrustVec.x << ' ' << thrustVec.y << ' ' << thrustVec.z << std::endl;
	std::cout << "Drag:" << drag.x << ' ' << drag.y << ' ' << drag.z << std::endl;

	float velForwardMag = glm::length(vel * vel * forward / glm::length(vel));
	glm::vec3 lift = (velUp * velForwardMag * velForwardMag) * cons.halfRo * wingArea * liftCoef(vel, forward);
	if (std::isnan(lift.x)) lift = glm::vec3(0.0f);

	std::cout << "Lift: "<<lift.x << ' ' << lift.y << " " << lift.z << std::endl << std::endl;

	glm::vec3 totalForce = thrustVec+drag+lift;
	glm::vec3 totalAccel = totalForce / mass;
	if (!grounded) totalAccel += cons.g;
	vel += totalAccel * timeDelta;
	pos += vel * timeDelta;
}


float liftCoef(glm::vec3 airstream, glm::vec3 forward)
{
	float angle = glm::acos(glm::dot(forward, airstream) / glm::length(airstream));
	if (glm::dot(glm::vec3(0, 0, 1), glm::cross(airstream, forward)) < 0) angle = -angle;

	std::cout << "Angle: " << angle << " - "<< glm::dot(glm::vec3(0, 0, 1), glm::cross(airstream, forward))<<" ";
	if ((angle > - cons.pi / 8 && angle < cons.pi / 8) || (angle > 7 * cons.pi / 8 && angle < cons.pi))
	{ 
		std::cout << glm::sin(6 * angle)<< std::endl;
		return 1.5*glm::sin(6 * angle);
	}
	std::cout << glm::sin(2 * angle) << std::endl;
	return 1.5 * glm::sin(2 * angle);
}

float sumOfVec3(glm::vec3 vec)
{
	return vec.x + vec.y + vec.z;
}

void PhysObject::checkCollision(GameObject* collider)
{
	if (pos.y - boundingBoxSize < collider->pos.y)
	{
		pos.y = collider->pos.y + boundingBoxSize;
		vel.y = 0;
		grounded = true;
	}
	grounded = false;
}