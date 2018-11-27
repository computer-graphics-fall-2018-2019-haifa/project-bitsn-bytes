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
		// Computed properties
		glm::mat4x4 transformation;
		glm::mat4x4 worldTransform;
		glm::mat4x4 normalTransformation;
		glm::vec3 centroid;
		glm::vec3 minCoordinates;
		glm::vec3 maxCoordinates;
		// Helper properties
		bool shouldRender;

	public:
		MeshModel(const std::string& primitive);
		MeshModel(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::string& modelName = "");
		virtual ~MeshModel();

		void SetWorldTransformation(const glm::mat4x4& worldTransform);
		const glm::mat4x4& GetWorldTransformation() const;

		const glm::vec4& GetColor() const;
		void SetColor(const glm::vec4& color);

		const std::string& GetModelName();

		void SetModelRenderingState(bool state) { shouldRender = state; }

		bool IsModelRenderingActive() { return shouldRender; }
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
		glm::vec4 GetCoordinates();
		void SetCoordinates(const glm::vec4& coordinates_);
};

#endif // !__MESHMODEL_H__
