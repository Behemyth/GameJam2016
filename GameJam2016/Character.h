#pragma once
#include "Object.h"
#include "NavMesh.h"

class Character :
	public Object
{
public:
	Character(float fps,int,  int, char*,bool, NavMesh*,float);
	~Character();
	void UpdatePosition();
	void Update(double dt);
	glm::vec3 normalizedDirection;
	NavMesh* nm;

private:
	float counter;
	float fps;
	float framesSize;
	float stancesSize;

	

	glm::vec3 destination;
	Face previous;

	std::vector<Face> path;

	bool isAI;
	int curStance;
	int curFrame;

};

