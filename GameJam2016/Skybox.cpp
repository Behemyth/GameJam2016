#include "Skybox.h"


Skybox::Skybox(Camera* cameraN,float height1,char* texname)
{
	height = height1;
	camera = cameraN;
	//isStatic = true;


	fragmentName = "fragment-shader[none].txt";
	//position = glm::translate(glm::mat4(),glm::vec3(0,5*METER,0));
	positionXYZ = glm::vec3(0.0f,-2.0*METER,0.0f);
	float height = METER*1.5f;
	sizeXYZ = glm::vec3(height,0.0f , height);
	ExtractFromFile("Grid.obj");
	//NormalizeScale(glm::vec3(height));


	textureName = texname;

	Load();
}

void Skybox::Update(double dt){
	positionXYZ.x = camera->position().x+3*METER;
	positionXYZ.z = camera->position().z-3*METER;

	rotationXYZ = glm::vec3(0.0f,1.0f,0.0f);
	rotation += dt*5.0f;
	/*position = glm::mat4();
	position *=camera->orientation();
	position = glm::translate(position, positionXYZ);*/

	//positionXYZ = glm::vec3(0.0f, 50.0*METER, 0.0f);
	Object::Update(dt);
}

Skybox::~Skybox()
{
}
