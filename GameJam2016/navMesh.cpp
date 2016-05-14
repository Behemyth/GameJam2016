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

float distance(Node a, Node b) {
	return sqrt(pow(a.v.position[0] - b.v.position[0], 2) + pow(a.v.position[1] - b.v.position[1], 2) + pow(a.v.position[2] - b.v.position[2], 2));
}

std::vector<Vertex> NavMesh::shortestPath(Vertex start, Vertex end) {
	std::priority_queue<Node, std::vector<Node>, sortF> openList;
	std::set<Node> closedList;

	Node first;
	first.v = start;
	first.f = 0;
	first.g = 0;
	Node last;
	last.v = end;
	first.h = distance(first, last);

	openList.push(first);

	while (!openList.empty()) {

	}


}


/*
find the node with the least f on the open list, call it "q"
pop q off the open list
generate q's 8 successors and set their parents to q
for each successor
if successor is the goal, stop the search
successor.g = q.g + distance between successor and q
successor.h = distance from goal to successor
successor.f = successor.g + successor.h

if a node with the same position as successor is in the OPEN list \
which has a lower f than successor, skip this successor
if a node with the same position as successor is in the CLOSED list \
which has a lower f than successor, skip this successor
otherwise, add the node to the open list
end
push q on the closed list
*/