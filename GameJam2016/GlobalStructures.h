#pragma once
//place all global structures here




// a point representing data of a point in space
typedef struct Vertex{
	glm::vec3 position; //d for double, i for int ect.
	glm::vec2 texCoord; 
	glm::vec3 normal;
}Vertex;

bool operator ==(const Vertex& x, const Vertex& y) {
	return (x.position[0] == y.position[0] && x.position[1] == y.position[1] && x.position[2] == y.position[2]);
}

//A single index struct has 3 indices to 3 vertices creating a triangle CW Order!!! for correct shading
typedef struct Face{
	glm::uvec3 indices;
}Index;