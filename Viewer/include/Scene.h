#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>
#include "MeshModel.h"
#include "Camera.h"
#include "Constants.h"

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

public:
	Scene();
	// Model related functions
	void AddModel(const std::shared_ptr<MeshModel>& model);
	const int GetModelCount() const;

	void SetActiveModelIndex(int index);
	const int GetActiveModelIndex() const;

	const unsigned int AddPrimitiveModel(PRIM_MODEL primitiveModel);

	// Camera related functions
	void AddCamera(const Camera& camera);
	const int GetCameraCount() const;

	void SetActiveCameraIndex(int index);
	const int GetActiveCameraIndex() const;

	Camera* GetActiveCamera();
	void NextCamera();
	void DeleteActiveCamera();
	const bool ShouldRenderCamera(int cameraIndex);

	// Transformation related functions
	void SetWorldTransformation(const glm::mat4x4 world);
	const glm::mat4x4 GetWorldTransformation();

};