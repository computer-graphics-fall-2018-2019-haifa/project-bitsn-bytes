#include "MeshModel.h"
#include "Utils.h"
#include "Constants.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

MeshModel::MeshModel(const MeshModel& primitive)
{
	faces = primitive.faces;
	vertices = primitive.vertices;
	normals = primitive.normals;
	modelName = primitive.modelName;
	transformation = primitive.transformation;
	worldTransformation = primitive.worldTransformation;
	normalTransformation = primitive.normalTransformation;
	minCoordinates = primitive.minCoordinates;
	maxCoordinates = primitive.maxCoordinates;
	vertexNormals = primitive.vertexNormals;
	vertexPositions = primitive.vertexPositions;
	centroid = primitive.centroid;
	cubeLines = primitive.cubeLines;
	color = primitive.color;
}

MeshModel::MeshModel(const std::vector<Face>& faces_, const std::vector<glm::vec3>& vertices_, const std::vector<glm::vec3>& normals_, const std::string& modelName_) :
	faces(faces_),
	vertices(vertices_),
	normals(normals_),
	modelName(modelName_),
	transformation(I_MATRIX),
	worldTransformation(I_MATRIX),
	normalTransformation(I_MATRIX),
	centroid({ 0, 0, 0 }),
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
	float absoluteMin = fmin(fmin(minCoordinates.x, minCoordinates.y), minCoordinates.z);
	float absoluteMax = fmax(fmax(maxCoordinates.x, maxCoordinates.y), maxCoordinates.z);

	glm::vec3 normalizedVector;
	unsigned int vertexPositionsCount = faces.size() * FACE_ELEMENTS;
	vertexPositions = new glm::vec3[vertexPositionsCount];
	unsigned int vertexNormalsCount = normals.size();
	vertexNormals = new glm::vec3[vertexNormalsCount];

	for (unsigned int i = 0; i < vertices.size(); i++) {
		normalizedVector.x = NORMALIZE_COORDS((vertices[i].x - centroid.x), absoluteMin, absoluteMax);
		normalizedVector.y = NORMALIZE_COORDS((vertices[i].y - centroid.y), absoluteMin, absoluteMax);
		normalizedVector.z = NORMALIZE_COORDS((vertices[i].z - centroid.z), absoluteMin, absoluteMax);

		vertices[i] = normalizedVector;
	}

	// Create triangles via iterating over the faces
	size_t index = 0;
	for each (Face face in faces) {
		for (int i = 0; i < FACE_ELEMENTS; i++) {
			int currentVertexIndex = face.GetVertexIndex(i);
			float x = vertices[currentVertexIndex - 1].x;
			float y = vertices[currentVertexIndex - 1].y;
			float z = vertices[currentVertexIndex - 1].z;
			vertexPositions[index++] = glm::vec3(x, y, z);
		}
	}

	for (unsigned int i = 0; i < vertexNormalsCount; i++) {
		vertexNormals[i] = normals[i];
	}

	// Calculate normalized centroid coordinates
	centroid.x = NORMALIZE_COORDS(0, absoluteMin, absoluteMax);
	centroid.y = NORMALIZE_COORDS(0, absoluteMin, absoluteMax);
	centroid.z = NORMALIZE_COORDS(0, absoluteMin, absoluteMax);
	// Calculate normalized minimum coordinates
	minCoordinates.x = NORMALIZE_COORDS(minCoordinates.x, absoluteMin, absoluteMax);
	minCoordinates.y = NORMALIZE_COORDS(minCoordinates.y, absoluteMin, absoluteMax);
	minCoordinates.z = NORMALIZE_COORDS(minCoordinates.z, absoluteMin, absoluteMax);
	// Calculate normalized maximum coordiantes
	maxCoordinates.x = NORMALIZE_COORDS(maxCoordinates.x, absoluteMin, absoluteMax);
	maxCoordinates.y = NORMALIZE_COORDS(maxCoordinates.y, absoluteMin, absoluteMax);
	maxCoordinates.z = NORMALIZE_COORDS(maxCoordinates.z, absoluteMin, absoluteMax);

	SetModelRenderingState(true);
	buildBorderCube(cubeLines);
}

MeshModel::~MeshModel()
{

}

std::pair<std::vector<glm::vec3>, std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>>>* MeshModel::Render()
{
	std::pair<std::vector<glm::vec3>, std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>>>* verticesData = new std::pair<std::vector<glm::vec3>, std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>>>();
	std::vector<glm::vec3> modelVertexPositions;
	std::vector<glm::vec3> modelVertices;
	std::vector<glm::vec3> modelVerticesNormals;
	unsigned int vertexPositionsCount = faces.size() * FACE_ELEMENTS;

	for (size_t i = 0; i < vertexPositionsCount; i++) {
		glm::vec3 vertexPosition = vertexPositions[i];
		modelVertexPositions.push_back(vertexPosition);
	}

	for (size_t i = 0; i < vertices.size(); i++) {
		glm::vec3 vertex = vertices[i];
		modelVertices.push_back(vertex);
	}

	for (size_t i = 0; i < normals.size(); i++) {
		glm::vec3 normal = normals[i];
		modelVerticesNormals.push_back(normal);
	}

	verticesData->first = modelVertexPositions;
	verticesData->second.first = modelVertices;
	verticesData->second.second = modelVerticesNormals;

	return verticesData;
}

std::vector<std::vector<glm::vec3>> MeshModel::GetModelTriangles()
{
	std::vector<std::vector<glm::vec3>> triangles;

	for (int i = 0; i < faces.size(); i++) {
		Face face = faces[i];
		glm::vec3 point_1 = vertices[face.GetVertexIndex(0) - 1];
		glm::vec3 point_2 = vertices[face.GetVertexIndex(1) - 1];
		glm::vec3 point_3 = vertices[face.GetVertexIndex(2) - 1];

		std::vector<glm::vec3> triangle = { point_1, point_2, point_3 };
		triangles.push_back(triangle);
	}

	return triangles;
}

void MeshModel::buildBorderCube(CUBE_LINES& cubeLines)
{
	INIT_CUBE_COORDINATES(maxCoordinates, minCoordinates);

	glm::vec3      RNB = { cRight , cBottom , cNear };	//            _________________________RFT=maxCoordinates(x,y,z)
	glm::vec3      RFB = { cRight , cBottom , cFar };	//           /LFT___________________  /|
	glm::vec3      LFB = { cLeft  , cBottom , cFar };	//          / / ___________________/ / |
	glm::vec3      LNB = { cLeft  , cBottom , cNear };  //         / / /| |               / /  |
	glm::vec3      RNT = { cRight , cTop    , cNear };	//        / / / | |              / / . |
	glm::vec3      RFT = { cRight , cTop    , cFar };	//       / / /| | |             / / /| |
	glm::vec3      LFT = { cLeft  , cTop    , cFar };	//      / / / | | |            / / / | |
	glm::vec3      LNT = { cLeft  , cTop    , cNear };  //     / / /  | | |           / / /| | |
														//    / /_/__________________/ / / | | |
	cubeLines.line[0] = { RNB, RFB };					//   /LNT___________________ _/ /  | | |
	cubeLines.line[1] = { RNB, LNB };					//   | ____________________RNT| |  | | |
	cubeLines.line[2] = { RNB, RNT };					//   | | |    | | |_________| | |__| | |
	cubeLines.line[3] = { RFB, LFB };					//   | | |    | |___________| | |____| |
	cubeLines.line[4] = { RFB, RFT };					//   | | |   / /LFB_________| | |_  / /RFB
	cubeLines.line[5] = { LFB, LNB };					//   | | |  / / /           | | |/ / /
	cubeLines.line[6] = { LFB, LFT };					//   | | | / / /            | | | / /
	cubeLines.line[7] = { LNB, LNT };					//   | | |/ / /             | | |/ /
	cubeLines.line[8] = { RNT, LNT };					//   | | | / /              | | ' /
	cubeLines.line[9] = { RNT, RFT };					//   | | |/_/_______________| |  /
	cubeLines.line[10] = { RFT, LFT };					//   | |____________________| | /
	cubeLines.line[11] = { LFT, LNT };					//   |________________________|/
														//   LNB=minCoordinates(u,v,w)   RNB   
}

void MeshModel::SetModelTransformation(const glm::mat4x4& transformation_)
{
	transformation = transformation_;
}

const glm::mat4x4 MeshModel::GetModelTransformation() const
{
	return transformation;
}

void MeshModel::SetWorldTransformation(const glm::mat4x4& worldTransformation_)
{
	worldTransformation = worldTransformation_;
}

const glm::mat4x4& MeshModel::GetWorldTransformation() const
{
	return worldTransformation;
}

void MeshModel::SetNormalTransformation(const glm::mat4x4& normalTransformation_)
{
	normalTransformation = normalTransformation_;
}

const glm::mat4x4 MeshModel::GetNormalTransformation() const
{
	return normalTransformation;
}

void MeshModel::SetColor(const glm::vec4& color_)
{
	color = color_;
}

const glm::vec4& MeshModel::GetColor() const
{
	return color;
}

const std::string& MeshModel::GetModelName() const
{
	return modelName;
}

// PrimMeshModel implementation

PrimMeshModel::PrimMeshModel(const PRIMITIVE primitive) : MeshModel(Utils::LoadMeshModel(PRIMITIVES.at(primitive)))
{

}

// CameraModel implementation
CameraModel::CameraModel(glm::vec4 coordinates_) : PrimMeshModel(CAMERA)
{
	coordinates = coordinates_;
}

const glm::vec4 CameraModel::GetCoordinates() const
{
	return coordinates;
}

void CameraModel::SetCoordinates(const glm::vec4& coordinates_)
{
	coordinates = coordinates_;
}