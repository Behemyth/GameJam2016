#include "Character.h"


Character::Character(float fps1)
{
	counter = 0;
	float height = 1.0f;
	fps = fps1;
	framesSize = 1.0f / 4.0f;
	stancesSize = 1.0f / 4.0f;
	curStance = 0;
	curFrame = 0;

	fragmentName = "fragment-shader[none].txt";

	GetVertices().push_back({ { -height / 2.0f, height, 0.0f }, { (curFrame*framesSize), ((curStance + 1)*stancesSize) }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 2.0f, height, 0.0f }, { ((curFrame + 1)*framesSize), ((curStance + 1)*stancesSize) }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { -height / 2.0f, 0.0f, 0.0f }, { (curFrame*framesSize), (curStance*stancesSize) }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 2.0f, 0.0f, 0.0f }, { ((curFrame + 1)*framesSize), (curStance*stancesSize) }, { 0.0f, 1.0f, 0.0f } });

	GetIndices().push_back({ glm::uvec3(2, 0, 1) });
	GetIndices().push_back({ glm::uvec3(2, 1, 3) });

	textureName = "CatSheet.png";
	Load();

}

void Character::Update(double dt){
	counter += fps*dt;

	if (counter>=1.0f){
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


	Object::Flush();
	Object::Update(dt);
}

Character::~Character()
{
}
