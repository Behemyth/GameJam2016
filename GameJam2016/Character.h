#pragma once
#include "Object.h"
class Character :
	public Object
{
public:
	Character(float fps,int,  int, char*,bool);
	~Character();
	void Update(double dt);
private:
	float counter;
	float fps;
	float framesSize;
	float stancesSize;

	glm::vec3 normalizedDirection;

	bool isAI;
	int curStance;
	int curFrame;

};

