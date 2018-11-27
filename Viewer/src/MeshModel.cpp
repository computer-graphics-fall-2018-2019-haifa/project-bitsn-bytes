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
	worldTransform(glm::mat4x4(1)),
	minCoordinates({ std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max() }),
	maxCoordinates({ -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max() })
{

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

	glm::vec3 normalizedVector;
	unsigned int verticesCount = vertices.size();
	glm::vec3* _vertices = new glm::vec3[verticesCount];
	unsigned int vertexPositionsCount = faces.size() * FACE_ELEMENTS;
	glm::vec3* vertexPositions = new glm::vec3[vertexPositionsCount];
	unsigned int vertexNormalsCount = normals.size();
	glm::vec3* vertexNormals = new glm::vec3[vertexNormalsCount];

	for (unsigned int i = 0; i < verticesCount; i++) {
		normalizedVector.x = NORMALIZE_COORDS((vertices[i].x - centroid.x), absoluteMin, absoluteMax);
		normalizedVector.y = NORMALIZE_COORDS((vertices[i].y - centroid.y), absoluteMin, absoluteMax);
		normalizedVector.z = NORMALIZE_COORDS((vertices[i].z - centroid.z), absoluteMin, absoluteMax);

		_vertices[i] = normalizedVector;
	}

	// Create triangles via iterating over the faces
	size_t index = 0;
	for each (Face face in faces) {
		for (int i = 0; i < FACE_ELEMENTS; i++) {
			int currentVertexIndex = face.GetVertexIndex(i);
			float x = _vertices[currentVertexIndex - 1].x;
			float y = _vertices[currentVertexIndex - 1].y;
			float z = _vertices[currentVertexIndex - 1].z;
			vertexPositions[index++] = glm::vec3(x, y, z);
		}
	}

	for (unsigned int i = 0; i < vertexNormalsCount; i++) {
		vertexNormals[i] = normals[i];
	}
	// Calculate normalized centroid coordinates
	centroid.x = NORMALIZE_COORDS(centroid.x, absoluteMin, absoluteMax);
	centroid.y = NORMALIZE_COORDS(centroid.y, absoluteMin, absoluteMax);
	centroid.z = NORMALIZE_COORDS(centroid.z, absoluteMin, absoluteMax);
	// Calculate normalized minimum coordinates
	minCoordinates.x = NORMALIZE_COORDS(minCoordinates.x, absoluteMin, absoluteMax);
	minCoordinates.y = NORMALIZE_COORDS(minCoordinates.y, absoluteMin, absoluteMax);
	minCoordinates.z = NORMALIZE_COORDS(minCoordinates.z, absoluteMin, absoluteMax);
	// Calculate normalized maximum coordiantes
	maxCoordinates.x = NORMALIZE_COORDS(maxCoordinates.x, absoluteMin, absoluteMax);
	maxCoordinates.y = NORMALIZE_COORDS(maxCoordinates.y, absoluteMin, absoluteMax);
	maxCoordinates.z = NORMALIZE_COORDS(maxCoordinates.z, absoluteMin, absoluteMax);
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