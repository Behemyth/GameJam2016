#include "Landscape.h"


Landscape::Landscape(char* name)
{

	ExtractFromFile(name);

	for (int i = 0; i < GetVertices().size(); i++){
		GetVertices()[i].texCoord *= 30;
	}

	textureName = "Ground.png";
	Load();
}


Landscape::~Landscape()
{
}
