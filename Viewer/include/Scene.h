#pragma once

#ifndef __SCENE_H__
#define __SCENE_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Camera.h"
//#include "Renderer.h"
#include "Constants.h"

class Renderer;

/*
 * Scene class.
 * This class holds all the scene information (models, cameras, lights, etc..)
 */
class Scene {
	private:
		std::vector<std::shared_ptr<MeshModel>> models;
		std::vector<Camera> cameras;

		int activeCameraIndex;
		int activeModelIndex;
		Renderer *renderer;

		glm::mat4x4 worldTransformation;

		bool drawVecNormal;
		bool drawFaceNormal;
		bool showBorderCube;

	public:
		Scene();
		Scene(Renderer *renderer_);
		// Model related functions
		void AddModel(const std::shared_ptr<MeshModel>& model);
		const int GetModelCount() const;

		void SetActiveModelIndex(int index);
		const int GetActiveModelIndex() const;

		const unsigned int AddPrimitiveModel(PRIMITIVE primitiveModel);

		void NextModel();
		void DeleteActiveModel();

		// Camera related functions
		void AddCamera(const Camera& camera);
		const int GetCameraCount() const;

		// Actions
		void SetActiveCameraIndex(int index);
		const int GetActiveCameraIndex() const;
		Camera* GetActiveCamera();
		void NextCamera();
		void DeleteActiveCamera();
		const bool ShouldRenderCamera(int cameraIndex);
		void ShowVerticesNormals(const bool key);
		void ShowFacesNormals(const bool key);
		void ShowBorderCube(const bool key);

		// Projection functions
		void SetOrthographicProjection(const PROJECTION_PARAMETERS);
		void SetPerspectiveProjection(const PERSPECTIVE_PARAMETERS);
		const glm::mat4x4 GetActiveCameraTransformation();
		const glm::mat4x4 GetActiveCameraProjection();

		// Scale function
		void ScaleActiveCamera(const float scaleFactor);
		void ScaleActiveModel(const float scaleFactor);

		// Translation functions
		void TranslateActiveCameraXAxis(const float factor);
		void TranslateActiveCameraYAxis(const float factor);
		void TranslateActiveCameraZAxis(const float factor);
		void TranslateActiveModelXAxis(const float factor);
		void TranslateActiveModelYAxis(const float factor);
		void TranslateActiveModelZAxis(const float factor);

		// Rotation functions
		void RotateActiveCameraXAxis(const float factor);
		void RotateActiveCameraYAxis(const float factor);
		void RotateActiveCameraZAxis(const float factor);
		void RotateActiveModelXAxis(const float factor);
		void RotateActiveModelYAxis(const float factor);
		void RotateActiveModelZAxis(const float factor);

		// Transformation related functions
		void SetWorldTransformation(const glm::mat4x4 world);
		const glm::mat4x4 GetWorldTransformation();
		glm::mat4x4 GetActiveModelTransformation();

};

#endif // !__SCENE_H__