#pragma once
#include "BasicIncludes.h"
#include "GlobalStructures.h"
#include "Object.h"

struct Node {
	Node* parent;
	Face v;
	float f, g, h;
	bool operator ==(const Node& y) const {
		return v == y.v;
	}
	bool operator <(const Node& y) const {
		return f < y.f;
	}
	Node& operator =(const Node& y) {
		f = y.f;
		g = y.g;
		h = y.h;
		parent = y.parent;
		v = y.v;
		return *this;
	}
};

class NavMesh: public Object {
public:
	NavMesh(char *);

	bool inMesh(glm::vec3& v);
	bool inFace(Face& f, glm::vec3& v);

	//A* FUNCTIONS ------------------------------------------------------------------------------
	bool areNeighbors(Face& a, Face& b);
	void shortestPathHelper(std::vector<Face>& path, Face& start, Face& end);
	std::vector<Face> shortestPath(glm::vec3& start, glm::vec3& end);
	int indexIndex(Face& v);
	glm::vec3 center(Face& a);
	float distance(Node& a, Node& b);
	std::vector<Face> findPath(std::vector<Face>& path, Node& first, Node& last);
	//--------------------------------------------------------------------------------------------

	//MOVEMENT FUNCTIONS ------------------------------------------------------------------------
	std::vector<Face> findNeighbors(glm::vec3);
	Face pointToFace(glm::vec3);
	//-------------------------------------------------------------------------------------------
private:
	std::vector< std::vector<Face> > neighbors;


};
