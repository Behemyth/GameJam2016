#pragma once
#include "Object.h"

class Skybox: public Object
{
public:
	Skybox(Camera*);
	~Skybox();
	Camera* camera;
	void Update(double);
private:

};

