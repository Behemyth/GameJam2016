#pragma once
#include "Object.h"
class Character :
	public Object
{
public:
	Character(float fps);
	~Character();
	void Update(double dt);
private:
	float counter;
	float fps;
	float framesSize;
	float stancesSize;

	int curStance;
	int curFrame;

};

