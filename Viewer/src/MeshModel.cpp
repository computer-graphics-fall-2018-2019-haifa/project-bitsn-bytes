#include "MeshModel.h"
#include "Utils.h"
#include "Constants.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

MeshModel::MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName) :
	modelName(modelName),
	worldTransform(glm::mat4x4(1))
{

	glm::vec3 minCoordinates = { numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max() };
	glm::vec3 maxCoordinates = { -numeric_limits<float>::max(), -numeric_limits<float>::max(), -numeric_limits<float>::max() };

	for (auto vertex : vertices) {
		// Find minimum coordinate values through all vertices
		minCoordinates.x = MIN(minCoordinates.x, vertex.x);
		minCoordinates.y = MIN(minCoordinates.y, vertex.y);
		minCoordinates.z = MIN(minCoordinates.z, vertex.z);
		// Find maximum coordinate values through all vertices
		maxCoordinates.x = MAX(maxCoordinates.x, vertex.x);
		maxCoordinates.y = MAX(maxCoordinates.y, vertex.y);
		maxCoordinates.z = MAX(maxCoordinates.z, vertex.z);
		// Calculate model centroid
		centroid += vertex;
	}

	centroid /= (float) vertices.size();
	minCoordinates -= centroid;
	maxCoordinates -= centroid;

	// Find absolute minimum and maximum
	float absoluteMin = MIN(minCoordinates.x, MIN(minCoordinates.y, minCoordinates.z));
	float absoluteMax = MAX(maxCoordinates.x, MAX(maxCoordinates.y, maxCoordinates.z));


	
}

MeshModel::~MeshModel()
{

}

void MeshModel::SetWorldTransformation(const glm::mat4x4& worldTransform)
{
	this->worldTransform = worldTransform;
}

const glm::mat4x4& MeshModel::GetWorldTransformation() const
{
	return worldTransform;
}

void MeshModel::SetColor(const glm::vec4& color)
{
	this->color = color;
}

const glm::vec4& MeshModel::GetColor() const
{
	return color;
}

const std::string& MeshModel::GetModelName()
{
	return modelName;
}