#define _USE_MATH_DEFINES

#include "Camera.h"
#include "Utils.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Camera::Camera() : viewTransformation(I_MATRIX), projectionTransformation(I_MATRIX)
{
	cameraModel = new CameraModel(HOMOGENEOUS_VECTOR4);
}

Camera::Camera(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up) :
	zoom(1.0),
	projectionTransformation(I_MATRIX)
{
	SetCameraLookAt(eye, at, up);
	cameraModel = new CameraModel(eye);
}

Camera::~Camera()
{
}

void Camera::SetCameraLookAt(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up)
{
	glm::vec3 eyeAtDirection = glm::normalize(eye - at);
	glm::vec3 fromUpAlongCameraTop = glm::cross(up, eyeAtDirection);
	glm::vec3 fromUpAlongCameraTopDirection = glm::normalize(fromUpAlongCameraTop);
	glm::vec3 cameraView = glm::cross(eyeAtDirection, fromUpAlongCameraTopDirection);
	glm::vec3 cameraViewDirection = glm::normalize(cameraView);
	glm::vec4 homogenousComponent = glm::vec4(HOMOGENEOUS_VECTOR4);
	glm::mat4x4 cameraViewTransformation = glm::mat4x4(Utils::ExpandToVec4(fromUpAlongCameraTopDirection),
		Utils::ExpandToVec4(cameraViewDirection),
		Utils::ExpandToVec4(eyeAtDirection),
		homogenousComponent);

	cameraViewTransformation[3][0] -= eye.x;
	cameraViewTransformation[3][1] -= eye.y;
	cameraViewTransformation[3][2] -= eye.z;

	viewTransformation = cameraViewTransformation;
}

void Camera::SetTransformation(const glm::mat4x4& transformation)
{
	viewTransformation = transformation;
}

void Camera::SetProjection(const glm::mat4x4& projection)
{
	projectionTransformation = projection;
}

void Camera::SetOrthographicProjection(const PROJECTION_PARAMETERS parameters)
{
	SET_PROJECTION_PARAMETERS(parameters);

	float width = right - left;
	float height = top - bottom;
	projectionTransformation = glm::ortho(-width / 2, width / 2, -height / 2, height / 2, zNear, zFar);
}

void Camera::SetPerspectiveProjection(const PERSPECTIVE_PARAMETERS parameters)
{
	SET_PERSPESCTIVE_PARAMETERS(parameters);

	this->projectionTransformation = glm::perspective(fovy, aspect, zNear, zFar);
}

void Camera::SetFrustumViewVolume(const PROJECTION_PARAMETERS parameters)
{
	validateProjectionParameters(parameters);

	SET_PROJECTION_PARAMETERS(parameters);

	projectionTransformation = glm::mat4x4(
		{
			{   2.0f * zNear / (right - left)    ,             0                    ,                  0                  ,              0              },
			{                0                 ,   2.0f * zNear / (top - bottom)    ,                  0                  ,              0              },
			{ (right + left) / (right - left)  , (top + bottom) / (top - bottom)  ,  -(zFar + zNear) / (zFar - zNear)   ,             -1              },
			{                0                 ,              0                   , -2.0f * zFar * zNear / (zFar - zNear)   ,              0              }
		});

	frustumParameters = parameters;

	throw false;
}

void Camera::SetZoom(const float zoom)
{

}

void Camera::SetCameraModel(CameraModel* model)
{
	cameraModel = model;
}

void Camera::SetModelRenderingState(bool state)
{
	cameraModel->SetModelRenderingState(state);
}

MeshModel* Camera::GetCameraModel()
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

void Camera::validateProjectionParameters(const PROJECTION_PARAMETERS parameters)
{
	SET_PROJECTION_PARAMETERS(parameters);

	if (right - left == 0 || top - bottom == 0 || zFar - zNear == 0)
	{
		throw true;
	}
}