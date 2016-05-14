#include "Landscape.h"


Landscape::Landscape(char* name)
{

	ExtractFromFile(name);


	textureName = "dirt.jpg";
	Load();
}


Landscape::~Landscape()
{
}
