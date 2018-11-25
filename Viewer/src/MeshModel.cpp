#include "MeshModel.h"
#include "Utils.h"
#include "Constants.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

MeshModel::MeshModel(const std::string& primitive)
{

}

MeshModel::MeshModel(const std::vector<Face>& faces_, const std::vector<glm::vec3>& vertices_, const std::vector<glm::vec3>& normals_, const std::string& modelName_) :
	faces(faces_),
	vertices(vertices_),
	normals(normals_),
	modelName(modelName_),
	worldTransform(glm::mat4x4(1))
{

	glm::vec3 minCoordinates = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
	glm::vec3 maxCoordinates = { -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() };

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

// PrimMeshModel implementation

PrimMeshModel::PrimMeshModel(const PRIMITIVE primitive) : MeshModel(PRIMITIVES.at(primitive))
{

}

// CameraModel implementation
CameraModel::CameraModel(glm::vec4 _coordinates) : PrimMeshModel(CAMERA)
{
	coordinates = _coordinates;
	this->SetModelRenderingState(false);
}

glm::vec4 CameraModel::GetCoordinates()
{
	return coordinates;
}

void CameraModel::SetCoordinates(const glm::vec4& coordinates_)
{
	coordinates = coordinates_;
}