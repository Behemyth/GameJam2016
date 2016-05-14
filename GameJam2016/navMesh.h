#include "BasicIncludes.h"
#include "GlobalStructures.h"
#include "Object.h"

struct Node {
	Node* parent;
	Face v;
	float f, g, h;
	bool operator ==(const Node& y) {
		return v == y.v;
	}
};

class NavMesh: public Object {
public:
	NavMesh();

	bool inMesh(const Vertex& v);
	bool inFace(const Face& f, const Vertex& v);

	//A* FUNCTIONS ------------------------------------------------------------------------------
	bool areNeighbors(const Face& a, const Face& b);
	void shortestPathHelper(std::vector<Face>& path, const Face& start, const Face& end);
	std::vector<Face> shortestPath(const Vertex& start, const Vertex& end);
	int indexIndex(const Face& v);
	glm::vec3 center(const Face& a);
	float distance(const Node& a, const Node& b);
	std::vector<Face> findPath(std::vector<Face>& path, const Node& first, const Node& last);
	//--------------------------------------------------------------------------------------------
private:
	std::vector< std::vector<Face> > neighbors;


};
