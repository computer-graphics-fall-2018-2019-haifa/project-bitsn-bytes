#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Camera.h"
#include "Renderer.h"
#include "Constants.h"

using namespace std;

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */
class Scene {
	private:
		vector<shared_ptr<MeshModel>> models;
		vector<Camera> cameras;

		int activeCameraIndex;
		int activeModelIndex;
		Renderer *renderer;

		glm::mat4x4 worldTransformation;

		bool drawVecNormal;
		bool drawFaceNormal;
		bool showBorderCube;

	public:
		Scene(Renderer *renderer);
		// Model related functions
		void AddModel(const shared_ptr<MeshModel>& model);
		const int GetModelCount() const;

		void SetActiveModelIndex(int index);
		const int GetActiveModelIndex() const;

		const unsigned int AddPrimitiveModel(PRIMITIVE_MODEL primitiveModel);

		// Camera related functions
		void AddCamera(const Camera& camera);
		const int GetCameraCount() const;

		void SetActiveCameraIndex(int index);
		const int GetActiveCameraIndex() const;

		Camera* GetActiveCamera();
		void NextCamera();
		void DeleteActiveCamera();
		const bool ShouldRenderCamera(int cameraIndex);

		void SetOrthographicProjection(const PROJECTION_PARAMETERS);
		void SetPerspectiveProjection(const PERSPECTIVE_PARAMETERS);

		// Transformation related functions
		void SetWorldTransformation(const glm::mat4x4 world);
		const glm::mat4x4 GetWorldTransformation();

};