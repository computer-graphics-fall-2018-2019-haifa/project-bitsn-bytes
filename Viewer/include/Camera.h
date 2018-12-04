#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <memory>
#include <glm/glm.hpp>
#include "MeshModel.h"
#include "Constants.h"

/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 *
 * Implementation suggestion:
 * --------------------------
 * Make the Camera class be a subclass of MeshModel, so you can easily and elegantly render 
 * the cameras you have added to the scene.
 */
class Camera
{
private:
	glm::mat4x4 viewTransformation;
	glm::mat4x4 projectionTransformation;
	CameraModel *cameraModel;
	PROJECTION_PARAMETERS frustumParameters;
	int cameraIndex;
	float zoom;

	void validateProjectionParameters(const PROJECTION_PARAMETERS parameters);

public:
	Camera();
	Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up);
	~Camera();

	std::pair<std::vector<glm::vec3>, std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>>>* Render() { return cameraModel->Render(); }

	void SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up);

	void SetTransformation(const glm::mat4x4& transformation);

	void SetProjection(const glm::mat4x4& projection);

	void SetOrthographicProjection(const PROJECTION_PARAMETERS parameters);

	void SetPerspectiveProjection(const PERSPECTIVE_PARAMETERS parameters);

	void SetFrustumViewVolume(const PROJECTION_PARAMETERS parameters);

	void SetZoom(const float zoom);

	void SetCameraModel(CameraModel* model);

	void SetModelRenderingState(bool state);

	void SetCameraIndex(int index) { cameraIndex = index; }

	const bool IsModelRenderingActive() { return cameraModel->IsModelRenderingActive(); }

	MeshModel* GetCameraModel();

	glm::mat4x4 GetTransformation();

	glm::mat4x4 GetProjection();
};

#endif // !__CAMERA_H__
