#include "ResourceManager.h"


ResourceManager::ResourceManager()
{
	root = GameObject();
	root.modelMatrix = glm::mat4(1.0f);
}

void ResourceManager::initShaders(const char* vertexSource, const char* vertexSource2, const char* fragmentSource)
{
	shader = new ShaderProgram(vertexSource, NULL, fragmentSource);
	complexShader = new ShaderProgram(vertexSource2, NULL, fragmentSource);
}

void ResourceManager::drawScene(glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Shaders
	shader->use();
	glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(perspectiveMatrix));
	glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(viewMatrix));

	//SimpleDraw
	for (GameObject* object : renderQueue)
	{
		if (!object->renderFlag) continue;

		glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(object->modelMatrix));
		glUniform1i(shader->u("textureMap0"), texturePalette[object->textureIndex].textureUnit);

		glEnableVertexAttribArray(shader->a("texCoord"));
		glVertexAttribPointer(shader->a("texCoord"), 2, GL_FLOAT, false, 0, modelPalette[object->modelIndex].texCoords);

		glEnableVertexAttribArray(shader->a("vertex"));
		glVertexAttribPointer(shader->a("vertex"), 4, GL_FLOAT, false, 0,modelPalette[object->modelIndex].vertices);
		
		glActiveTexture(GL_TEXTURE0+texturePalette[object->textureIndex].textureUnit);
		glBindTexture(GL_TEXTURE_2D, texturePalette[object->textureIndex].texture);

		glDrawArrays(GL_TRIANGLES, 0,modelPalette[object->modelIndex].vertexCount);
		
		glDisableVertexAttribArray(shader->a("vertex"));
		glDisableVertexAttribArray(shader->a("texCoord"));
	}
	
	complexShader->use();
	glUniformMatrix4fv(complexShader->u("P"), 1, false, glm::value_ptr(perspectiveMatrix));
	glUniformMatrix4fv(complexShader->u("V"), 1, false, glm::value_ptr(viewMatrix));

	//complex draw
	for (GameObject* object : renderQueue2)
	{
		if (!object->renderFlag) continue;
		glUniformMatrix4fv(complexShader->u("M"), 1, false, glm::value_ptr(object->modelMatrix));
		glUniform1i(complexShader->u("textureMap0"), texturePalette[object->textureIndex].textureUnit);
		glActiveTexture(GL_TEXTURE0 + texturePalette[object->textureIndex].textureUnit);
		glBindTexture(GL_TEXTURE_2D, texturePalette[object->textureIndex].texture);

		modelPalette2[object->modelIndex].draw();
	}
}



simpleModel::simpleModel(float* vertices, int vertexCount, float* texCoords)
{
	this->vertices = vertices;
	this->vertexCount = vertexCount;
	this->texCoords = texCoords;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->indices = indices;
	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

int ResourceManager::addModel(float* vertices, int vertexCount, float* texCoords)
{
	modelPalette.push_back(simpleModel(vertices, vertexCount, texCoords));
	return modelPalette.size() - 1;
}


int ResourceManager::loadModel(const char* modelSource)
{
	Model model;
	
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(modelSource, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Error during assimp model loading: " << import.GetErrorString() << std::endl;
		return 0;
	}
	//directory = path.substr(0, path.find_last_of('/'));

	model.processNode(scene->mRootNode, scene);

	modelPalette2.push_back(model);
	return modelPalette2.size() - 1;
}

void Model::draw()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		glBindVertexArray(meshes[i].VAO);
		glDrawElements(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}



void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 temp;
		temp.x = mesh->mVertices[i].x;
		temp.y = mesh->mVertices[i].y;
		temp.z = mesh->mVertices[i].z;
		vertex.position = temp;

		temp.x = mesh->mNormals[i].x;
		temp.y = mesh->mNormals[i].y;
		temp.z = mesh->mNormals[i].z;
		vertex.normal = temp;

		

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else vertex.texCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}
	// process indices
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		//
	}

	return Mesh(vertices, indices);
}

GLuint ResourceManager::readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	std::cout << error << std::endl;
	//Import do pamiêci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamiêci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}

int ResourceManager::addTexture(const char* textureSource, const char* specSource)
{
	texturePalette.push_back(Texture());
	int size = texturePalette.size() - 1;
	texturePalette[size].texture = readTexture(textureSource);
	if(specSource != "") texturePalette[size].specular = readTexture(specSource);
	texturePalette[size].textureUnit = size;
	return size;
}