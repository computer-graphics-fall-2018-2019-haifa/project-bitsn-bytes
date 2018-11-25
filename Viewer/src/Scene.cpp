#include "Scene.h"
#include "MeshModel.h"
#include "Constants.h"
#include "Camera.h"
#include <string>

Scene::Scene() : activeCameraIndex(DISABLED), activeModelIndex(DISABLED), worldTransformation(I_MATRIX), drawVecNormal(false)
{

}

Scene::Scene(Renderer *renderer_) : activeCameraIndex(DISABLED), activeModelIndex(DISABLED), worldTransformation(I_MATRIX), drawVecNormal(false), renderer(renderer_)
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
	return 1;
}

// Camera related functions implementation
void Scene::AddCamera(const Camera& camera)
{
	cameras.push_back(camera);
}

const int Scene::GetCameraCount() const
{
	return cameras.size();
}

void Scene::SetActiveCameraIndex(int index)
{
	if (index >= 0 && (unsigned int) index < cameras.size())
	{
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
		return &cameras[activeCameraIndex];
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
		Camera* _activeCamera = &cameras[activeCameraIndex];
		return _activeCamera->GetCameraModel()->IsModelRenderingActive();
	}
}

void Scene::SetOrthographicProjection(PROJECTION_PARAMETERS)
{

}

void Scene::SetPerspectiveProjection(PERSPECTIVE_PARAMETERS)
{

}

const glm::mat4x4 Scene::GetActiveCameraTransformation()
{
	return glm::mat4x4{ {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0} };
}

const glm::mat4x4 Scene::GetActiveCameraProjection()
{
	return glm::mat4x4{ {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0} };
}

void Scene::ScaleActiveCamera(const float scaleFactor)
{
	
}

void Scene::TranslateActiveCameraXAxis(const float moveFactor)
{

}

void Scene::TranslateActiveCameraYAxis(const float moveFactor)
{

}

void Scene::TranslateActiveCameraZAxis(const float moveFactor)
{

}

void Scene::RotateActiveCameraXAxis(const float moveFactor)
{

}

void Scene::RotateActiveCameraYAxis(const float moveFactor)
{

}

void Scene::RotateActiveCameraZAxis(const float moveFactor)
{

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