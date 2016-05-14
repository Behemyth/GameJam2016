#pragma once
#include "Object.h"

class Skybox: public Object
{
public:
	Skybox(Camera*,float,char *);
	~Skybox();
	Camera* camera;
	void Update(double);
private:
	float height;
};

