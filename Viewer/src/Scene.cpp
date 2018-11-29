#include "Scene.h"
#include "MeshModel.h"
#include "Constants.h"
#include "Camera.h"
#include <string>

Scene::Scene() : activeCameraIndex(DISABLED), activeModelIndex(DISABLED), worldTransformation(I_MATRIX), drawVerticesNormals(false)
{

}

// Model related functions implementation
void Scene::AddModel(const std::shared_ptr<MeshModel>& model)
{
	models.push_back(model);
}

const int Scene::GetModelCount() const
{
	return models.size();
}

void Scene::SetActiveModelIndex(int index)
{
	if (index >= 0 && (unsigned int) index < models.size())
	{
		activeModelIndex = index;
	}
}

const int Scene::GetActiveModelIndex() const
{
	return activeModelIndex;
}

const unsigned int Scene::AddPrimitiveModel(PRIMITIVE primitiveModel)
{
	std::shared_ptr<MeshModel> newModel = std::make_shared<MeshModel>(PrimMeshModel(primitiveModel));
	models.push_back(newModel);
	return models.size() - 1;
}

void Scene::NextModel()
{
	if (activeModelIndex != DISABLED) {
		activeModelIndex = (activeModelIndex + 1) % models.size();
	}
}

void Scene::DeleteActiveModel()
{
	if (activeModelIndex != DISABLED) {
		models.erase(models.begin() + activeModelIndex);
		activeModelIndex = models.size() - 1;
	}
}

// Camera related functions implementation
void Scene::AddCamera(Camera* camera)
{
	cameras.push_back(camera);
}

const int Scene::GetCameraCount() const
{
	return cameras.size();
}

void Scene::SetActiveCameraIndex(int index)
{
	if (index >= 0 && (unsigned int) index < cameras.size()) {
		activeCameraIndex = index;
	}
}

const int Scene::GetActiveCameraIndex() const
{
	return activeCameraIndex;
}

Camera* Scene::GetActiveCamera()
{
	if (activeCameraIndex != DISABLED) {
		return cameras[activeCameraIndex];
	}

	return NULL;
}

void Scene::NextCamera()
{
	if (activeCameraIndex != DISABLED) {
		activeCameraIndex = (activeCameraIndex + 1) % cameras.size();
	}
}

void Scene::DeleteActiveCamera()
{
	if (activeCameraIndex != DISABLED) {
		cameras.erase(cameras.begin() + activeCameraIndex);
		activeCameraIndex = cameras.size() - 1;
	}
}

const bool Scene::ShouldRenderCamera(int cameraIndex)
{
	if (activeCameraIndex != DISABLED) {
		Camera* _activeCamera = cameras[activeCameraIndex];
		return _activeCamera->GetCameraModel()->IsModelRenderingActive();
	}
}

void Scene::SetOrthographicProjection(const PROJECTION_PARAMETERS parameters)
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		activeCamera->SetOrthographicProjection(parameters);
	}
}

void Scene::SetPerspectiveProjection(const PERSPECTIVE_PARAMETERS parameters)
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		activeCamera->SetPerspectiveProjection(parameters);
	}
}

void Scene::SetFrustumViewVolume(const PROJECTION_PARAMETERS parameters)
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		activeCamera->SetFrustumViewVolume(parameters);
	}
}

const glm::mat4x4 Scene::GetActiveCameraTransformation()
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		return activeCamera->GetTransformation();
	}
	else {
		return ZERO_MATRIX;
	}
}

const glm::mat4x4 Scene::GetActiveCameraProjection()
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		return activeCamera->GetProjection();
	}
	else {
		return ZERO_MATRIX;
	}
}

void Scene::ScaleActiveCamera(const float scaleFactor)
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		glm::mat4x4 currentTransformation = activeCamera->GetTransformation();
		glm::mat4x4 scaleTransformation(SCALING_MATRIX4(scaleFactor));
		activeCamera->SetTransformation(scaleTransformation * currentTransformation);
	}
}

void Scene::TranslateActiveCameraXAxis(const float translationFactor)
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		glm::mat4x4 currentTransformation = activeCamera->GetTransformation();
		glm::mat4x4 translateTransformation(TRANSLATION_MATRIX(translationFactor, 0, 0));
		activeCamera->SetTransformation(translateTransformation * currentTransformation);
	}
}

void Scene::TranslateActiveCameraYAxis(const float translationFactor)
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		glm::mat4x4 currentTransformation = activeCamera->GetTransformation();
		glm::mat4x4 translateTransformation(TRANSLATION_MATRIX(0, translationFactor, 0));
		activeCamera->SetTransformation(translateTransformation * currentTransformation);
	}
}

void Scene::TranslateActiveCameraZAxis(const float translationFactor)
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		glm::mat4x4 currentTransformation = activeCamera->GetTransformation();
		glm::mat4x4 translateTransformation(TRANSLATION_MATRIX(0, 0, translationFactor));
		activeCamera->SetTransformation(translateTransformation * currentTransformation);
	}
}

void Scene::RotateActiveCameraXAxis(const float rotationFactor)
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		glm::mat4x4 currentTransformation = activeCamera->GetTransformation();
		glm::mat4x4 origin(TRANSLATION_MATRIX(-currentTransformation[3][0], -currentTransformation[3][1], -currentTransformation[3][2]));
		glm::mat4x4 distination(TRANSLATION_MATRIX(currentTransformation[3][0], currentTransformation[3][1], currentTransformation[3][2]));
		glm::mat4x4 rotateTransformation(ROTATING_MATRIX_X_AXIS(rotationFactor));
		activeCamera->SetTransformation(distination * rotateTransformation * origin * currentTransformation);
	}
}

void Scene::RotateActiveCameraYAxis(const float rotationFactor)
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		glm::mat4x4 currentTransformation = activeCamera->GetTransformation();
		glm::mat4x4 origin(TRANSLATION_MATRIX(-currentTransformation[3][0], -currentTransformation[3][1], -currentTransformation[3][2]));
		glm::mat4x4 distination(TRANSLATION_MATRIX(currentTransformation[3][0], currentTransformation[3][1], currentTransformation[3][2]));
		glm::mat4x4 rotateTransformation(ROTATING_MATRIX_Y_AXIS(rotationFactor));
		activeCamera->SetTransformation(distination * rotateTransformation * origin * currentTransformation);
	}
}

void Scene::RotateActiveCameraZAxis(const float rotationFactor)
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = cameras[activeCameraIndex];
		glm::mat4x4 currentTransformation = activeCamera->GetTransformation();
		glm::mat4x4 origin(TRANSLATION_MATRIX(-currentTransformation[3][0], -currentTransformation[3][1], -currentTransformation[3][2]));
		glm::mat4x4 distination(TRANSLATION_MATRIX(currentTransformation[3][0], currentTransformation[3][1], currentTransformation[3][2]));
		glm::mat4x4 rotateTransformation(ROTATING_MATRIX_Z_AXIS(rotationFactor));
		activeCamera->SetTransformation(distination * rotateTransformation * origin * currentTransformation);
	}
}

void Scene::TranslateActiveModelXAxis(const float translationFactor)
{
	if (activeModelIndex != DISABLED) {
		std::shared_ptr<MeshModel> activeModel = models[activeModelIndex];
		glm::mat4x4 currentTransformation = activeModel->GetWorldTransformation();
		glm::mat4x4 translateTransformation(TRANSLATION_MATRIX(translationFactor, 0, 0));
		activeModel->SetModelTransformation(translateTransformation * currentTransformation);
	}
}

void Scene::TranslateActiveModelYAxis(const float translationFactor)
{
	if (activeModelIndex != DISABLED) {
		std::shared_ptr<MeshModel> activeModel = models[activeModelIndex];
		glm::mat4x4 currentTransformation = activeModel->GetWorldTransformation();
		glm::mat4x4 translateTransformation(TRANSLATION_MATRIX(0, translationFactor, 0));
		activeModel->SetModelTransformation(translateTransformation * currentTransformation);
	}
}

void Scene::TranslateActiveModelZAxis(const float translationFactor)
{
	if (activeModelIndex != DISABLED) {
		std::shared_ptr<MeshModel> activeModel = models[activeModelIndex];
		glm::mat4x4 currentTransformation = activeModel->GetWorldTransformation();
		glm::mat4x4 translateTransformation(TRANSLATION_MATRIX(0, 0, translationFactor));
		activeModel->SetModelTransformation(translateTransformation * currentTransformation);
	}
}

void Scene::RotateActiveModelXAxis(const float rotationFactor)
{
	if (activeModelIndex != DISABLED) {
		std::shared_ptr<MeshModel> activeModel = models[activeModelIndex];
		glm::mat4x4 currentTransformation = activeModel->GetWorldTransformation();
		glm::mat4x4 origin(TRANSLATION_MATRIX(-currentTransformation[3][0], -currentTransformation[3][1], -currentTransformation[3][2]));
		glm::mat4x4 distination(TRANSLATION_MATRIX(currentTransformation[3][0], currentTransformation[3][1], currentTransformation[3][2]));
		glm::mat4x4 rotateTransformation(ROTATING_MATRIX_X_AXIS(rotationFactor));
		activeModel->SetModelTransformation(distination * rotateTransformation * origin * currentTransformation);
	}
}

void Scene::RotateActiveModelYAxis(const float rotationFactor)
{
	if (activeModelIndex != DISABLED) {
		std::shared_ptr<MeshModel> activeModel = models[activeModelIndex];
		glm::mat4x4 currentTransformation = activeModel->GetWorldTransformation();
		glm::mat4x4 origin(TRANSLATION_MATRIX(-currentTransformation[3][0], -currentTransformation[3][1], -currentTransformation[3][2]));
		glm::mat4x4 distination(TRANSLATION_MATRIX(currentTransformation[3][0], currentTransformation[3][1], currentTransformation[3][2]));
		glm::mat4x4 rotateTransformation(ROTATING_MATRIX_Y_AXIS(rotationFactor));
		activeModel->SetModelTransformation(distination * rotateTransformation * origin * currentTransformation);
	}
}

void Scene::RotateActiveModelZAxis(const float rotationFactor)
{
	if (activeModelIndex != DISABLED) {
		std::shared_ptr<MeshModel> activeModel = models[activeModelIndex];
		glm::mat4x4 currentTransformation = activeModel->GetWorldTransformation();
		glm::mat4x4 origin(TRANSLATION_MATRIX(-currentTransformation[3][0], -currentTransformation[3][1], -currentTransformation[3][2]));
		glm::mat4x4 distination(TRANSLATION_MATRIX(currentTransformation[3][0], currentTransformation[3][1], currentTransformation[3][2]));
		glm::mat4x4 rotateTransformation(ROTATING_MATRIX_Z_AXIS(rotationFactor));
		activeModel->SetModelTransformation(distination * rotateTransformation * origin * currentTransformation);
	}
}

// Transformation related functions

void Scene::SetWorldTransformation(const glm::mat4x4 world)
{
	worldTransformation = world;
}

const glm::mat4x4 Scene::GetWorldTransformation()
{
	return worldTransformation;
}

void Scene::ShowVerticesNormals(const bool key)
{
	drawVerticesNormals = key;
}

void Scene::ShowFacesNormals(const bool key)
{
	drawFacesNormals = key;
}

void Scene::ShowBorderCube(const bool key)
{
	drawBorderCube = key;
}

void Scene::ScaleActiveModel(const float scaleFactor)
{
	if (activeModelIndex != DISABLED) {
		std::shared_ptr<MeshModel> activeModel = models[activeModelIndex];
		glm::mat4x4 currentTransformation = activeModel->GetModelTransformation();
		glm::mat4x4 scaleTransformation(SCALING_MATRIX4(scaleFactor));
		activeModel->SetModelTransformation(scaleTransformation * currentTransformation);
	}
}

glm::mat4x4 Scene::GetActiveModelTransformation()
{
	if (activeModelIndex != DISABLED) {
		std::shared_ptr<MeshModel> activeModel = models[activeModelIndex];
		return activeModel->GetModelTransformation();
	}
	else
	{
		return ZERO_MATRIX;
	}
}

std::vector<std::shared_ptr<MeshModel>>& Scene::GetModels()
{
	return models;
}

std::vector<Camera*> Scene::GetCameras()
{
	return cameras;
}