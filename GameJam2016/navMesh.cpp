#include <vector>
#include <queue>
#include <set>
#include <math.h>
#include "NavMesh.h"
#include "GlobalStructures.h"

struct Node {
	Node* parent;
	Face v;
	float f, g, h;
};
NavMesh::NavMesh(){

	ExtractFromFile("BasicNav.obj");

	for (int i = 0; i < GetIndices().size(); i++) {
		for (int j = 0; j < GetIndices().size(); j++) {
			std::vector<Face> neighborsOfi;
			if ( areNeighbors(GetIndices()[i], GetIndices()[j]) ) {
				neighborsOfi.push_back(GetIndices()[j]);
			}
			neighbors.push_back(neighborsOfi);
		}
	}

	textureName = "dirt.jpg";
	Load();
}

bool NavMesh::areNeighbors(const Face& a, const Face& b) {
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

glm::vec3 NavMesh::center(const Face& i) {
	float x = (GetVertices()[i.indices[0]].position.x + GetVertices()[i.indices[1]].position.x + GetVertices()[i.indices[2]].position.x) / 3;
	float y = (GetVertices()[i.indices[0]].position.y + GetVertices()[i.indices[1]].position.y + GetVertices()[i.indices[2]].position.y) / 3;
	float z = (GetVertices()[i.indices[0]].position.z + GetVertices()[i.indices[1]].position.z + GetVertices()[i.indices[2]].position.z) / 3;
	return glm::vec3(x, y, z);
}

float NavMesh::distance(const Node& a, const Node& b) {
	glm::vec3 amid = center(a.v);
	glm::vec3 bmid = center(b.v);
	return sqrt(pow(amid.x - bmid.x, 2) + pow(amid.y - bmid.y, 2) + pow(amid.z - bmid.z, 2));
}

int NavMesh::indexIndex(const Face& i) {
	for (int x = 0; x < GetIndices().size(); x++) {
		for (int v = 0; v < 3; v++) {
			if (GetVertices()[GetIndices()[x].indices[v]] != GetVertices()[i.indices[v]]) {
				break;
			}
			return x;
		}
	}
	return -1;
}

std::vector<Face> NavMesh::shortestPath(const Face& start, const Face& end) {
	std::priority_queue<Node, std::vector<Node>, sortF> openList;
	std::set<Node> closedList;
	std::vector<Face> path;

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
		std::vector<Face> children = neighbors[indexIndex(q.v)];
		for (int i = 0; i < children.size(); i++) {
			Node child;
			child.v = children[i];
			child.parent = &q;
			for (int v = 0; v < 3; v++) {
				if () {
					break;
				}
				return path;
			}

			
		}
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