#pragma once
#include "Object.h"
#include "NavMesh.h"

class Character :
	public Object
{
public:
	Character(float fps, int, int, char*, bool, NavMesh*, float, Character*, char*, char*, irrklang::ISoundEngine* );
	~Character();
	void UpdatePosition();
	void Update(double dt);
	glm::vec3 normalizedDirection;
	NavMesh* nm;
	uint amount; 
	bool forward;
	float timeCounter;
	bool end;
	irrklang::ISoundEngine* sound;
	char* bSound;
	bool finally;
	char* fSound;
private:
	
	bool once;
	int thisTrigger;
	Character* mainC;
	Character* tail;
	Character* parent;
	float counter;
	float fps;
	float framesSize;
	float stancesSize;

	std::vector<glm::vec2> posStorage;

	glm::vec3 destination;
	Face previous;

	std::vector<Face> path;

	bool isAI;
	bool follow;
	int curStance;
	int curFrame;

};

