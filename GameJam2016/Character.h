#pragma once
#include "Object.h"
#include "NavMesh.h"

class Character :
	public Object
{
public:
	Character(float fps,int,  int, char*,bool, NavMesh*,float);
	~Character();
	void Update(double dt);
	glm::vec3 normalizedDirection;
private:
	float counter;
	float fps;
	float framesSize;
	float stancesSize;

	NavMesh* nm;

	glm::vec3 normalizedDirection;
	glm::vec3 destination;

	std::vector<Face> path;

	bool isAI;
	int curStance;
	int curFrame;

};

