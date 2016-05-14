#include "BasicIncludes.h"
#include "GlobalStructures.h"
#include "Object.h"

class NavMesh: public Object {
public:
	NavMesh();
	std::vector<Vertex> shortestPath(Vertex start, Vertex end);
private:
	std::vector< std::vector<Index> > neighbors;


};
