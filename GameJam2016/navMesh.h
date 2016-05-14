#include <vector>

class NavMesh {
public:
	NavMesh();
	std::vector<Vertex> shortestPath(Vertex start, Vertex end);
private:
	std::vector<Vertex> vertices;
	std::vector<Face> faces;

};
