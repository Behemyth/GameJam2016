#pragma once

#include "BasicIncludes.h"
#include "ShaderSupport.h"
#include "Camera.h"
#include "Texture.h"

//This class represents anything that is drawable onscreen. 
class Object
{
public:
	Object();
	~Object();

	virtual void Draw(Camera&);
	virtual void Load();
	virtual void Update(double );
	void Flush();
	glm::vec3 GetPosition();
	glm::mat4 GetMatrix();

	virtual void UpdatePosition();
	void ExtractFromFile(const char* name);

	glm::vec3 positionXYZ;
	glm::vec3 rotationXYZ;
	float rotation;
	glm::vec3 sizeXYZ;

	std::vector<Face>& GetIndices();
	std::vector<Vertex>& GetVertices();
	bool isGhost;

protected:
	bool isStatic;

	glm::mat4 position;
	glm::mat4 positionNormal;
	char* textureName;
	char* displacementName;
	char* normalName;
	char* fragmentName;
	char* vertexName;

	void NormalizeScale(glm::vec3);

private:
	std::vector<Vertex> vertices;
	std::vector<Face> indices;

	Texture* texture;
	Texture* displacement;
	Texture* normal;

	Bitmap LoadBmp(const char*);
	Texture* LoadTexture(Bitmap);

	//a matrix defining rotation, translation, and scaling of the object
	//only use if the vertices are object space and not world space
	
	GLuint cameraUniform;
	GLuint posUniform;
	GLuint texUniform;
	GLuint disUniform;
	GLuint normUniform;
	GLuint posNormUniform;

	shading::ShaderSupport* shader;
	GLuint vao;
	GLuint vbo;
	GLuint ibo;

	
};

