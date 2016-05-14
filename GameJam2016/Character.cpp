#include "Character.h"
#include "rand.h"


Character::Character(float fps1,int frameS,int stanceS,char* texName,bool AI, NavMesh* n,float sizeN)
{
	nm = n;

	counter = 0;
	isAI = AI;

	float height = 1.0f;
	fps = fps1;
	framesSize = 1.0f / frameS;
	stancesSize = 1.0f / stanceS;
	curStance = 0;
	curFrame = 0;

	fragmentName = "fragment-shader[none].txt";

	rotationXYZ = glm::vec3(1.0f, 0.0f, 1.0f);
	rotation = -45.0f;
	sizeXYZ = glm::vec3(sizeN);
	position = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
	destination = glm::vec3(0.0f, 0.0f, 0.0f);
	positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);


	GetVertices().push_back({ { -height / 2.0f, height, 0.0f }, { (curFrame*framesSize), ((curStance + 1)*stancesSize) }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 2.0f, height, 0.0f }, { ((curFrame + 1)*framesSize), ((curStance + 1)*stancesSize) }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { -height / 2.0f, 0.0f, 0.0f }, { (curFrame*framesSize), (curStance*stancesSize) }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 2.0f, 0.0f, 0.0f }, { ((curFrame + 1)*framesSize), (curStance*stancesSize) }, { 0.0f, 1.0f, 0.0f } });

	GetIndices().push_back({ glm::uvec3(2, 0, 1) });
	GetIndices().push_back({ glm::uvec3(2, 1, 3) });

	textureName = texName;
	Load();

}

void Character::Update(double dt){
	counter += fps*dt;

	if (isAI){
		if (positionXYZ == destination || path.size() <= 0) {
			std::uniform_int_distribution<int> distro(0, nm->GetVertices().size()-1);
			int vertexNum = GetDistribution(distro);
			destination = nm->GetVertices()[vertexNum].position;
			//destination = glm::vec3(0.2, 0.2, 0.2);
			path = nm->shortestPath(positionXYZ, destination);
		}
		glm::vec3 nextDest;
		if (path.size() > 0) {
			nextDest = nm->center(path.front());
			path.erase(path.begin());
		}
		if (path.size() == 0) {
			nextDest = destination;
		}
		normalizedDirection = glm::normalize(nextDest - positionXYZ);
		positionXYZ = positionXYZ + (normalizedDirection * float(dt) * float(0.1*METER));
	}
	else{
		normalizedDirection = glm::normalize(normalizedDirection);
	}
	if (glm::length(normalizedDirection) > 0.0000001){
		if (normalizedDirection.x >= 0 && normalizedDirection.z >= 0){
			curStance = 3;
		}
		else if (normalizedDirection.x >= 0 && normalizedDirection.z <= 0){
			curStance = 0;
		}
		else if (normalizedDirection.x <= 0 && normalizedDirection.z >= 0){
			curStance = 1;
		}
		else{
			curStance = 2;
		}




		if (counter >= 1.0f){
		counter = 0.0f;
		curFrame += 1;
		if (curFrame >= glm::round(1.0f / framesSize)){
			curFrame = 0;
		}

		GetVertices()[0].texCoord = glm::vec2((curFrame*framesSize), ((curStance + 1)*stancesSize));
		GetVertices()[1].texCoord = glm::vec2(((curFrame + 1)*framesSize), ((curStance + 1)*stancesSize));
		GetVertices()[2].texCoord = glm::vec2((curFrame*framesSize), (curStance*stancesSize));
		GetVertices()[3].texCoord = glm::vec2(((curFrame + 1)*framesSize), (curStance*stancesSize));

	}
	}

	Object::Flush();
	Object::Update(dt);
}

Character::~Character()
{
}
