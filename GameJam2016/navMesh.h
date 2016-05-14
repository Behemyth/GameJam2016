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
};

class NavMesh: public Object {
public:
	NavMesh();

	bool inMesh(Vertex& v);
	bool inFace(Face& f, Vertex& v);

	//A* FUNCTIONS ------------------------------------------------------------------------------
	bool areNeighbors(Face& a, Face& b);
	void shortestPathHelper(std::vector<Face>& path, Face& start, Face& end);
	std::vector<Face> shortestPath(Vertex& start, Vertex& end);
	int indexIndex(Face& v);
	glm::vec3 center(Face& a);
	float distance(Node& a, Node& b);
	std::vector<Face> findPath(std::vector<Face>& path, Node& first, Node& last);
	//--------------------------------------------------------------------------------------------
private:
	std::vector< std::vector<Face> > neighbors;


};
