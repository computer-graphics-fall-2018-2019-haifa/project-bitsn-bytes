#pragma once
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "Face.h"

using namespace std;

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
		string modelName;
		vector<Face> faces;
		vector<glm::vec3> vertices;
		vector<glm::vec3> normals;
		// Computed properties
		glm::mat4x4 transformation;
		glm::mat4x4 worldTransform;
		glm::mat4x4 normalTransformation;
		glm::vec3 centroid;
		// Helper properties
		bool shouldRender;

	public:
		MeshModel(const vector<Face>& faces, const vector<glm::vec3>& vertices, const vector<glm::vec3>& normals, const string& modelName = "");
		virtual ~MeshModel();

		void SetWorldTransformation(const glm::mat4x4& worldTransform);
		const glm::mat4x4& GetWorldTransformation() const;

		const glm::vec4& GetColor() const;
		void SetColor(const glm::vec4& color);

		const string& GetModelName();

		void SetModelRenderingState(bool state) { shouldRender = state; }

		bool IsModelRenderingActive() { return shouldRender; }
};
