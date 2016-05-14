#include "Character.h"


Character::Character()
{
	float height = 1.0f;

	GetVertices().push_back({ { -height / 2.0f, height, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 2.0f, height, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { -height / 2.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 2.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } });

	GetIndices().push_back({ glm::uvec3(2, 0, 1) });
	GetIndices().push_back({ glm::uvec3(2, 1, 3) });

	textureName = "dirt.jpg";
	Load();

}


Character::~Character()
{
}
