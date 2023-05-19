#include "ResourceManager.h"


ResourceManager::ResourceManager()
{
	root = GameObject();
	root.modelMatrix = glm::mat4(1.0f);
}

void ResourceManager::initShaders(const char* vertexSource, const char* fragmentSource)
{
	shader = new ShaderProgram(vertexSource, NULL, fragmentSource);
}

void ResourceManager::drawScene(glm::mat4 viewMatrix, glm::mat4 perspectiveMatrix)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Shaders
	shader->use();

	glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(perspectiveMatrix));
	glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(viewMatrix));


	//Inloop
	//glm::mat4 modelMatrix = glm::mat4();
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
}

Model::Model(float* vertices, int vertexCount, float* texCoords)
{
	this->vertices = vertices;
	this->vertexCount = vertexCount;
	this->texCoords = texCoords;
}

int ResourceManager::addModel(float* vertices, int vertexCount, float* texCoords)
{
	modelPalette.push_back(Model(vertices, vertexCount, texCoords));
	return modelPalette.size() - 1;
}

GLuint ResourceManager::readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	//Wczytanie do pamiêci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
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