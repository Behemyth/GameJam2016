#include "BasicIncludes.h"
#include "GlobalStructures.h"
#include "Object.h"

class NavMesh: public Object {
public:
	NavMesh();
	bool isNeighbor(const Index& a, const Index& b);
	std::vector<Vertex> shortestPath(Vertex start, Vertex end);
private:
	std::vector< std::vector<Index> > neighbors;


};
