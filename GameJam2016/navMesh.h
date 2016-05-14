#include "BasicIncludes.h"
#include "GlobalStructures.h"
#include "Object.h"

class NavMesh: public Object {
public:
	NavMesh();
	bool areNeighbors(const Index& a, const Index& b);
	std::vector<Vertex> shortestPath(Vertex start, Vertex end);
	int vertexIndex(Vertex v);
private:
	std::vector< std::vector<Index> > neighbors;


};
