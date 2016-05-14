#include "Skybox.h"


Skybox::Skybox()
{
	isGhost = true;
	isStatic = true;

	fragmentName = "fragment-shader[none].txt";
	//position = glm::translate(glm::mat4(),glm::vec3(0,5*METER,0));

	float height = KILOMETER*20.0f;
	ExtractFromFile("Sphere.obj");
	NormalizeScale(glm::vec3(height));


	textureName = "skybox.png";

	Load();
}

void Skybox::Update(double dt){

	Object::Update(dt);
}
Skybox::~Skybox()
{
}
