#include <vector>
#include <queue>
#include <set>
#include <math.h>
#include "NavMesh.h"
#include "GlobalStructures.h"

struct Node {
	Vertex v;
	float f, g, h;
};
NavMesh::NavMesh(){

	ExtractFromFile("BasicNav.obj");

	textureName = "dirt.jpg";
	Load();
}
struct sortF {
	bool operator() (const Node& a, const Node& b) {
		return a.f < b.f;
	}
};

std::vector<Vertex> NavMesh::shortestPath(Vertex start, Vertex end) {
	std::priority_queue<Node, std::vector<Node>, sortF> openList;
	std::set<Node> closedList;

	Node first;
	first.v = start;
	first.f = 0;
	first.g = 0;
	first.h = 
}

float distance(Node a, Node b) {
	return sqrt(pow(a.v.position[0] - b.v.position[0], 2) + pow(a.v.position[1] - b.v.position[1], 2) + pow(a.v.position[2] - b.v.position[2], 2))
}