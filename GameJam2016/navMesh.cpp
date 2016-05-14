#include <vector>
#include <queue>
#include <set>
#include <math.h>
#include <map>
#include "NavMesh.h"
#include "GlobalStructures.h"


NavMesh::NavMesh(char * name){
	isGhost = true;

	ExtractFromFile(name);

	for (int i = 0; i < GetIndices().size(); i++) {
		std::vector<Face> neighborsOfi;
		for (int j = 0; j < GetIndices().size(); j++) {
			if ( areNeighbors(GetIndices()[i], GetIndices()[j]) ) {
				neighborsOfi.push_back(GetIndices()[j]);
			}
		}
		neighbors.push_back(neighborsOfi);
		std::cout << neighborsOfi.size() << std::endl;
	}
	textureName = "dirt.jpg";
	Load();
}

bool NavMesh::inMesh(glm::vec3& v) {
	for (int i = 0; i < GetIndices().size(); i++) {
		if (inFace(GetIndices()[i], v)) {
			return true;
		}
	}
	return false;
}


bool NavMesh::inFace(Face& f, glm::vec3& p) {
	// Compute vectors
	glm::vec3 a = GetVertices()[f.indices.x].position;
	glm::vec3 b = GetVertices()[f.indices.y].position;
	glm::vec3 c = GetVertices()[f.indices.z].position;
	
	glm::vec3 v0 = c - a;
	glm::vec3 v1 = b - a;
	glm::vec3 v2 = p - a;

	// Compute dot products
	float dot00 = dot(v0, v0);
	float dot01 = dot(v0, v1);
	float dot02 = dot(v0, v2);
	float dot11 = dot(v1, v1);
	float dot12 = dot(v1, v2);

	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	return (u >= 0) && (v >= 0) && (u + v < 1);

}

bool NavMesh::areNeighbors(Face& a, Face& b) {
	if (a == b) {
		return false;
	}
	for (int i = 0; i < 3; i++) {
		if (GetVertices()[a.indices[i]] == GetVertices()[b.indices[0]]) {
			return true;
		}
		if (GetVertices()[a.indices[i]] == GetVertices()[b.indices[1]]) {
			return true;
		}
		if (GetVertices()[a.indices[i]] == GetVertices()[b.indices[2]]) {
			return true;
		}
	}
	return false;
}

struct sortF {
	bool operator() (const Node& a, const Node& b) const {
		return a.f < b.f;
	}
};

glm::vec3 NavMesh::center(Face& i) {
	float x = (GetVertices()[i.indices[0]].position.x + GetVertices()[i.indices[1]].position.x + GetVertices()[i.indices[2]].position.x) / 3;
	float y = (GetVertices()[i.indices[0]].position.y + GetVertices()[i.indices[1]].position.y + GetVertices()[i.indices[2]].position.y) / 3;
	float z = (GetVertices()[i.indices[0]].position.z + GetVertices()[i.indices[1]].position.z + GetVertices()[i.indices[2]].position.z) / 3;
	/*std::cout << "FACE:" << std::endl;
	std::cout << GetVertices()[i.indices.x].position.x << ", " << GetVertices()[i.indices.x].position.y << ", " << GetVertices()[i.indices.x].position.x << std::endl;
	std::cout << GetVertices()[i.indices.y].position.x << ", " << GetVertices()[i.indices.y].position.y << ", " << GetVertices()[i.indices.y].position.x << std::endl;
	std::cout << GetVertices()[i.indices.z].position.x << ", " << GetVertices()[i.indices.z].position.y << ", " << GetVertices()[i.indices.z].position.x << std::endl;
	std::cout << x << ", " << y << ", " << z << std::endl;*/
	return glm::vec3(x, y, z);
}

float NavMesh::distance(Node& a, Node& b) {
	glm::vec3 amid = center(a.v);
	glm::vec3 bmid = center(b.v);
	return glm::distance(amid, bmid);
}

/*bool NavMesh::faceEquals(const Face& a, const Face& b) {
	for (int v = 0; v < 3; v++) {
		if (GetVertices()[a.indices[v]] != GetVertices()[b.indices[v]]) {
			return false;
		}
		return true;
	}
} */

int NavMesh::indexIndex(Face& i) {
	for (int x = 0; x < GetIndices().size(); x++) {
		if (GetIndices()[x] == i ) {
			return x;
		}
	}
	return -1;
}

void printNode(Node& n) {
	std::cout << n.v.indices[0] << ", " << n.v.indices[1] << ", " << n.v.indices[2] << std::endl;
}

std::vector<Face> NavMesh::findPath(std::vector<Face>& path, Node& first, Node& last) {
	Node q = last;
	std::cout << "first: ";  printNode(first);
	std::cout << "last: ";  printNode(last);
	while (q.parent != NULL) {
		printNode(q);
		path.push_back(q.v);
		q = *(q.parent);
	}
	path.push_back(q.v);
	std::reverse(path.begin(), path.end());
	return path;
}

std::vector<Face> NavMesh::findPath2(std::vector<Face>& path, std::map<Node, Node*> m, Node& last){
	path.push_back(last.v);
	Node q = last;
	while (m.find(q) != m.end()) {
		Node temp = *m[q];
		m.erase(m.find(q));
		q = temp;
		path.push_back(q.v);
	}
	std::reverse(path.begin(), path.end());
	return path;
}

void NavMesh::shortestPathHelper(std::vector<Face>& path, Face& start, Face& end) {
	std::set<Node, sortF> openList;
	std::set<Node> closedList;

	Node first;
	first.v = start;
	first.f = 0;
	first.g = 0;
	first.parent = NULL;
	Node last;
	last.v = end;
	first.h = distance(first, last);

	openList.insert(first);
	Node q;
	while (!openList.empty()) {
		q = *(openList.begin());
		openList.erase(openList.begin());
		closedList.insert(q);
		std::vector<Face> children = neighbors[indexIndex(q.v)];
		std::vector<Node> childrenNodes;
		Node child;
		for (int i = 0; i < children.size(); i++) {
			child.v = children[i];
			if (child.v == last.v) {
				findPath(path, first, child);
				return;
			}
			if (closedList.find(child) != closedList.end()) {
				continue;
			}
			std::set<Node>::iterator itr = openList.find(child); 
			if (itr != openList.end() && (*itr).g > child.g) {
				Node n = *itr;
				openList.erase(itr);
				n.parent = &q;
				n.g = child.g;
				n.f = child.f;
				openList.insert(n);
			}
			else if (itr != openList.end()) {
				continue;
			}
			else {
				openList.insert(child);
				child.parent = &q;
				child.g = q.g + distance(child, q);
				child.h = distance(child, last);
				child.f = child.g + child.h;
			}

			//if ( (openList.find(child) != openList.end()) && ((*openList.find(child)).f < child.f) ) {
			//	continue;
			//}
		}
	} 
	std::cout << "FAILED TO FIND PATH" << std::endl;

}


std::vector<Face> NavMesh::shortestPath(glm::vec3& start, glm::vec3& end) {
	std::vector<Face> path;
	Face first;
	Face last;

	for (int i = 0; i < GetIndices().size(); i++) {
		if (inFace(GetIndices()[i], start)) {
			first = GetIndices()[i];
		}
		if (inFace(GetIndices()[i], end)) {
			last = GetIndices()[i];
		}
	}

	shortestPathHelper(path, first, last);
	return path;

}

