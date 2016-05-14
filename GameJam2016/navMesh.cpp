#include <vector>
#include "NavMesh.h"

NavMesh::NavMesh(){

	ExtractFromFile("BasicNav.obj");

	textureName = "dirt.jpg";
	Load();
}
std::vector<Vertex> NavMesh::shortestPath(Vertex start, Vertex end) {

}