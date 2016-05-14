#include <vector>
#include <queue>
#include <set>
#include <math.h>
#include "NavMesh.h"
#include "GlobalStructures.h"


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

/*bool NavMesh::faceEquals(const Face& a, const Face& b) {
	for (int v = 0; v < 3; v++) {
		if (GetVertices()[a.indices[v]] != GetVertices()[b.indices[v]]) {
			return false;
		}
		return true;
	}
} */

int NavMesh::indexIndex(const Face& i) {
	for (int x = 0; x < GetIndices().size(); x++) {
		if (GetIndices()[x] == i ) {
			return x;
		}
	}
	return -1;
}

std::vector<Face> NavMesh::findPath(std::vector<Face>& path, const Node& first, const Node& last) {
	Node q = last;
	while (!(q == first)) {
		path.push_back(q.v);
		q = *(q.parent);
	}
	path.push_back(q.v);

}

std::vector<Face> NavMesh::shortestPath(const Face& start, const Face& end) {
	std::set<Node, sortF> openList;
	std::set<Node> closedList;
	std::vector<Face> path;

	Node first;
	first.v = start;
	first.f = 0;
	first.g = 0;
	Node last;
	last.v = end;
	first.h = distance(first, last);

	openList.insert(first);

	while (!openList.empty()) {
		Node q = *(openList.begin());
		openList.erase(openList.begin());;
		std::vector<Face> children = neighbors[indexIndex(q.v)];
		for (int i = 0; i < children.size(); i++) {
			Node child;
			child.v = children[i];
			child.parent = &q;
			if (child.v == last.v) {
				findPath(path, first, last);
				return path;
			}
			child.g = q.g + distance(child, q);
			child.h = distance(child, last);
			child.f = child.g + child.h;

			if ( (openList.find(child) != openList.end()) && ((*openList.find(child)).f < child.f) ) {
				continue;
			}
			if ((closedList.find(child) != closedList.end()) && ((*closedList.find(child)).f < child.f)) {
				continue;
			}
			openList.insert(child);
		}
		closedList.insert(q);
	}


}

