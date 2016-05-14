#include <vector>
#include <queue>
#include <set>
#include <math.h>
#include "NavMesh.h"
#include "GlobalStructures.h"

struct Node {
	Node* parent;
	Vertex v;
	float f, g, h;
};
NavMesh::NavMesh(){

	ExtractFromFile("BasicNav.obj");

	for (int i = 0; i < GetIndices().size(); i++) {
		for (int j = 0; j < GetIndices().size(); j++) {
			std::vector<Index> neighborsOfi;
			if ( areNeighbors(GetIndices()[i], GetIndices()[j]) ) {
				neighborsOfi.push_back(GetIndices()[j]);
			}
			neighbors.push_back(neighborsOfi);
		}
	}

	textureName = "dirt.jpg";
	Load();
}

bool NavMesh::areNeighbors(const Index& a, const Index& b) {
	for (int i = 0; i < 3; i++) {
		if (GetVertices()[a.indices[i]] == GetVertices()[b.indices[i]]) {
			return true;
		}
		return false;
	}
}

struct sortF {
	bool operator() (const Node& a, const Node& b) {
		return a.f < b.f;
	}
};

float distance(const Node& a, const Node& b) {
	return sqrt(pow(a.v.position.x - b.v.position.x, 2) + pow(a.v.position.y - b.v.position.y, 2) + pow(a.v.position.z - b.v.position.z, 2));
}

int NavMesh::vertexIndex(Vertex v) {
	for (int i = 0; i < GetVertices().size(); i++) {
		if (GetVertices()[i] == v) {
			return i;
		}
	}
	return -1;
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
		Node q = openList.top();
		openList.pop();
		std::vector<Index> children = neighbors[vertexIndex(q.v)];

	}


}


/*
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