#define _USE_MATH_DEFINES

#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera() : viewTransformation(I_MATRIX), projectionTransformation(I_MATRIX)
{
	//cameraModel = new CameraModel();
}

Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	zoom(1.0)
{
	SetCameraLookAt(eye, at, up);
}

Camera::~Camera()
{
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{

}

void Camera::SetTransformation(const glm::mat4x4& transformation)
{

}

void Camera::SetOrthographicProjection(const PROJECTION_PARAMETERS parameters)
{

}

void Camera::SetPerspectiveProjection(const PERSPECTIVE_PARAMETERS parameters)
{

}

void Camera::SetFrustumViewVolume(const PROJECTION_PARAMETERS parameters)
{

}

void Camera::SetZoom(const float zoom)
{

}

void Camera::SetCameraModel(CameraModel* model)
{
	cameraModel = model;
}

CameraModel* Camera::GetCameraModel()
{
	return cameraModel;
}

glm::mat4x4 Camera::GetTransformation()
{
	return viewTransformation;
}

glm::mat4x4 Camera::GetProjection()
{
	return projectionTransformation;
}