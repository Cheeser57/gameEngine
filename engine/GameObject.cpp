#include "GameObject.h"

GameObject::GameObject( GameObject* parent,
						int modelIndex,	// -1 -> no model
						int textureIndex,
						glm::vec3 pos,
						glm::vec3 scale,
						glm::vec3 rot,
						float rotAngle,
	                    bool renderFlag)
{
	this->parent = parent;
	if(parent != nullptr) parent->addChild(this);
	
	this->pos = pos;
	this->rot = rot;
	this->rotAngle = rotAngle;
	this->scale = scale;

	this->modelIndex = modelIndex;
	this->textureIndex = textureIndex;

	if (modelIndex != -1)
	{
		this->renderFlag = renderFlag;
		updateModelMatrix();
	}
	else this->renderFlag = false;
}


void GameObject::updateSelfModelMatrixS()
{
	modelMatrix = parent->modelMatrix;
	
	modelMatrix = glm::rotate(modelMatrix,glm::radians(rotAngle), rot);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::scale(modelMatrix, scale);
}

void GameObject::updateModelMatrix()
{
	modelMatrix = parent->modelMatrix;

	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotAngle), rot);
	modelMatrix = glm::translate(modelMatrix, pos);
	modelMatrix = glm::scale(modelMatrix, scale);

	for (GameObject* child : children)
	{
		child->updateModelMatrix();
	}
	
}


void GameObject::addChild(GameObject* child)
{
	children.push_back(child);
}