#pragma once
//place all global structures here




// a point representing data of a point in space
typedef struct Vertex{
	glm::vec3 position; //d for double, i for int ect.
	glm::vec2 texCoord; 
	glm::vec3 normal;

	bool operator ==(const Vertex& y) const {
		return (position[0] == y.position[0] && position[1] == y.position[1] && position[2] == y.position[2]);
	}
	bool operator !=(const Vertex& y) const {
		return (position[0] != y.position[0] || position[1] != y.position[1] || position[2] != y.position[2]);
	}
}Vertex;

//A single index struct has 3 indices to 3 vertices creating a triangle CW Order!!! for correct shading
typedef struct Face{
	glm::uvec3 indices;
	bool operator ==(const Face& y) const {
		return (indices[0] == y.indices[0] && indices[1] == y.indices[1] && indices[2] == y.indices[2]);
	}
}Face;