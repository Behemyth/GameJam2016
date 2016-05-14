#include "Character.h"
#include "rand.h"


Character::Character(float fps1, int frameS, int stanceS, char* texName, bool AI, NavMesh* n, float sizeN, Character* mainC1, char* bSound1, char* fSound1, irrklang::ISoundEngine* soundN)
{
	once = true;
	sound = soundN;
	bSound = bSound1;
	fSound = fSound1;
	thisTrigger = 0;
	end = false;
	amount = 0;
	timeCounter = 0;
	forward = true;
	parent = this;
	tail = this;
	follow = false;
	mainC = mainC1;
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

	sizeXYZ = glm::vec3(sizeN);

	if (!AI){
		mainC = this;
		positionXYZ = glm::vec3(0.0f, height/2.0f, 0.0f);
	}
	else{
	std::uniform_int_distribution<int> distro(0, nm->GetIndices().size() - 1);
	int faceNum = GetDistribution(distro);
	Face start = nm->GetIndices()[faceNum];
	positionXYZ = nm->center(start);
	previous = nm->pointToFace(positionXYZ);
	destination = positionXYZ;
	}


	GetVertices().push_back({ { -height / 2.0f, height, 0.0f }, { (curFrame*framesSize), ((curStance + 1)*stancesSize) }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 2.0f, height, 0.0f }, { ((curFrame + 1)*framesSize), ((curStance + 1)*stancesSize) }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { -height / 2.0f, 0.0f, 0.0f }, { (curFrame*framesSize), (curStance*stancesSize) }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 2.0f, 0.0f, 0.0f }, { ((curFrame + 1)*framesSize), (curStance*stancesSize) }, { 0.0f, 1.0f, 0.0f } });

	GetIndices().push_back({ glm::uvec3(2, 0, 1) });
	GetIndices().push_back({ glm::uvec3(2, 1, 3) });

	textureName = texName;
	Load();

}

bool vecsEqual(glm::vec3 a, glm::vec3 b) {
	float eps = 0.1f;
	if (a.x > b.x - eps && a.x < b.x + eps) {
		if (a.y > b.y - eps && a.y < b.y + eps) {
			if (a.z>b.z - eps && a.z < b.z + eps) {
				return true;
			}
		}
	}
	return false;
}

void Character::Update(double dt){
	counter += fps*dt;

	if (this == mainC&&mainC->amount == 9){
		mainC->forward = false;
	}
	if (mainC->forward){
		if (follow){
			normalizedDirection = glm::normalize(parent->GetPosition() - positionXYZ);
			float dist = glm::distance(parent->GetPosition(), positionXYZ);
			positionXYZ = positionXYZ + ((normalizedDirection * float(dt) * float(0.3*METER))) * dist;
		}

		if (isAI){

			if (glm::distance(mainC->GetPosition(), positionXYZ) < METER*0.1f){
				isAI = false;
				follow = true;
				mainC->amount++;
				parent = mainC->tail;
				mainC->tail = this;
				thisTrigger = posStorage.size();
				

				irrklang::ISound* s = sound->play3D(bSound, irrklang::vec3df(0.0f, 0.0f, 0.0f), false, false, true);
			}

			if (vecsEqual(destination, positionXYZ)) {
				std::vector<Face> neighbors = nm->findNeighbors(positionXYZ);
				Face nextFace;

				//std::cout << neighbors.size();
				if (neighbors.size() == 1) {
					//	std::cout << "here" << std::endl;
					nextFace = previous;
				}
				else {
					do {
						std::uniform_int_distribution<int> distro(0, neighbors.size() - 1);
						int vertexNum = GetDistribution(distro);
						nextFace = neighbors[vertexNum];
					} while (nextFace == previous);
				}
				destination = nm->center(nextFace);
				previous = nextFace;
				//std::cout << positionXYZ.x << " " << positionXYZ.y << " " << positionXYZ.z
				//	<< ", " << destination.x << " " << destination.y << " " << destination.z << std::endl;
			}
			normalizedDirection = glm::normalize(destination - positionXYZ);
			positionXYZ = positionXYZ + (normalizedDirection * float(dt) * float(0.15*METER));


			/*if (positionXYZ == destination || path.size() <= 0) {
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
				positionXYZ = positionXYZ + (normalizedDirection * float(dt) * float(0.1*METER));*/


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
	}
//	position = glm::translate(glm::mat4(), positionXYZ);
////	position = glm::rotate(position, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 1.0f));
//	position = glm::rotate(position, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//	position = glm::scale(position, sizeXYZ);

	if (mainC->forward){
		posStorage.push_back(glm::vec2(positionXYZ.x, positionXYZ.z));
	}
	else{
		int p = posStorage.size() - 1 - int(mainC->timeCounter);


		if (p <= thisTrigger&&once){
			once = false;
			irrklang::ISound* s = sound->play3D(fSound, irrklang::vec3df(0.0f, 0.0f, 0.0f), false, false, true);

		}
		if (p <= 0){
			mainC->end = true;
		}

		glm::vec2 pos = posStorage[p];
		positionXYZ.x = pos.x;
		positionXYZ.z = pos.y;
		mainC->timeCounter += 0.20;
	}

	Object::Flush();
	Object::Update(dt);
}

void Character::UpdatePosition(){
	position = glm::translate(glm::mat4(), positionXYZ);
	position = glm::rotate(position, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	position = glm::rotate(position, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	position = glm::scale(position, sizeXYZ);
}
Character::~Character()
{
}
