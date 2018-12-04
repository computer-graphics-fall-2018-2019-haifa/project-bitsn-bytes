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
		std::vector<Camera*> cameras;

		int activeCameraIndex;
		int activeModelIndex;

		glm::mat4x4 worldTransformation;

		bool drawVerticesNormals;
		bool drawFacesNormals;
		bool drawBorderCube;

	public:
		Scene();

		// Model related functions
		void AddModel(const std::shared_ptr<MeshModel>& model);
		const int GetModelCount() const;
		void SetActiveModelIndex(int index);
		const int GetActiveModelIndex() const;
		void AddPrimitiveModel(PRIMITIVE primitiveModel);
		void NextModel();
		void DeleteActiveModel();
		glm::mat4x4 GetActiveModelTransformation();
		std::vector<std::shared_ptr<MeshModel>>& GetModels();

		// Camera related functions
		void AddCamera(Camera* camera);
		const int GetCameraCount() const;
		void SetActiveCameraIndex(int index);
		const int GetActiveCameraIndex() const;
		Camera* GetActiveCamera();
		void NextCamera();
		void DeleteActiveCamera();
		const bool ShouldRenderCamera(int cameraIndex);
		const glm::mat4x4 GetActiveCameraTransformation();
		const glm::mat4x4 GetActiveCameraProjection();
		std::vector<Camera*> GetCameras();

		// Actions
		void ShowVerticesNormals(const bool key);
		void ShowFacesNormals(const bool key);
		void ShowBorderCube(const bool key);
		bool ShouldShowVerticesNormals() { return drawVerticesNormals; }
		bool ShouldShowFacesNormals() { return drawFacesNormals; }
		bool ShouldShowBorderCube() { return drawBorderCube; }

		// Projection functions
		void SetOrthographicProjection(const PROJECTION_PARAMETERS);
		void SetPerspectiveProjection(const PERSPECTIVE_PARAMETERS);
		void SetFrustumViewVolume(const PROJECTION_PARAMETERS);

		// Scale function
		void ScaleActiveCamera(const float scaleFactor);
		void ScaleActiveModel(const float scaleFactor);

		// Translation functions
		void TranslateActiveCameraXAxis(const float translationFactor);
		void TranslateActiveCameraYAxis(const float translationFactor);
		void TranslateActiveCameraZAxis(const float translationFactor);
		void TranslateActiveModelXAxis(const float translationFactor);
		void TranslateActiveModelYAxis(const float translationFactor);
		void TranslateActiveModelZAxis(const float translationFactor);

		// Rotation functions
		void RotateActiveCameraXAxis(const float rotationFactor);
		void RotateActiveCameraYAxis(const float rotationFactor);
		void RotateActiveCameraZAxis(const float rotationFactor);
		void RotateActiveModelXAxis(const float rotationFactor);
		void RotateActiveModelYAxis(const float rotationFactor);
		void RotateActiveModelZAxis(const float rotationFactor);

		// Transformation related functions
		void SetWorldTransformation(const glm::mat4x4 world);
		const glm::mat4x4 GetWorldTransformation();
};

#endif // !__SCENE_H__