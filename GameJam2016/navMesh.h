#include "BasicIncludes.h"
#include "GlobalStructures.h"
#include "Object.h"

struct Node {
	Node* parent;
	Face v;
	float f, g, h;
};

class NavMesh: public Object {
public:
	NavMesh();
	bool areNeighbors(const Face& a, const Face& b);
	std::vector<Face> shortestPath(const Face& start, const Face& end);
	int indexIndex(const Face& v);
	glm::vec3 center(const Face& a);
	float distance(const Node& a, const Node& b);
private:
	std::vector< std::vector<Face> > neighbors;


};
