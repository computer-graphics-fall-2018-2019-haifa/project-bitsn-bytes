#pragma once

#ifndef __MESHMODEL_H__
#define __MESHMODEL_H__

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Face.h"
//#include "Utils.h"
#include "Constants.h"

/*
 * MeshModel class.
 * This class represents a mesh model (with faces and normals informations).
 * You can use Utils::LoadMeshModel to create instances of this class from .obj files.
 */
class MeshModel
{
	private:
		// constant properties
		glm::vec4 color;
		std::string modelName;
		std::vector<Face> faces;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		glm::vec3* vertexPositions;
		glm::vec3* vertexNormals;
		// Computed properties
		glm::mat4x4 transformation;
		glm::mat4x4 worldTransformation;
		glm::mat4x4 normalTransformation;
		glm::vec3 centroid;
		glm::vec3 minCoordinates;
		glm::vec3 maxCoordinates;
		CUBE_LINES cubeLines;
		// Helper properties
		bool shouldRender;

	public:
		MeshModel(const MeshModel& primitive);
		MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
		virtual ~MeshModel();

		std::pair<std::vector<glm::vec3>, std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>>>* Render();

		void SetModelTransformation(const glm::mat4x4& tranformation_);
		const glm::mat4x4 GetModelTransformation() const;

		void SetWorldTransformation(const glm::mat4x4& worldTransformation_);
		const glm::mat4x4& GetWorldTransformation() const;

		void SetNormalTransformation(const glm::mat4x4& normalTransformation);
		const glm::mat4x4 GetNormalTransformation() const;

		const glm::vec4& GetColor() const;
		void SetColor(const glm::vec4& color);

		const std::string& GetModelName() const;

		void SetModelRenderingState(bool state) { shouldRender = state; }

		bool IsModelRenderingActive() { return shouldRender; }

		glm::vec3 GetCentroid() { return centroid; }

		CUBE_LINES& GetBorderCube() { return cubeLines; }
};

class PrimMeshModel : public MeshModel
{
	private:
		PRIMITIVE model;

	public:
		PrimMeshModel(const PRIMITIVE primitive);
};

class CameraModel : public PrimMeshModel
{
	private:
		const PRIMITIVE model = CAMERA;
		glm::vec4 coordinates;

	public:
		CameraModel(glm::vec4 coordinates);
		const glm::vec4 GetCoordinates() const;
		void SetCoordinates(const glm::vec4& coordinates_);
};

#endif // !__MESHMODEL_H__
