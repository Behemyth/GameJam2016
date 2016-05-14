#pragma once
#include "Object.h"
class Character :
	public Object
{
public:
	Character(float fps,int,  int, char*,bool);
	~Character();
	void Update(double dt);
	glm::vec3 normalizedDirection;
private:
	float counter;
	float fps;
	float framesSize;
	float stancesSize;

	bool isAI;
	int curStance;
	int curFrame;

};

