#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	activeCameraIndex(0),
	activeModelIndex(0)
{

}

void Scene::AddModel(const std::shared_ptr<MeshModel>& model)
{
	models.push_back(model);
}

const int Scene::GetModelCount() const
{
	return models.size();
}

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
	// implementation suggestion...
	if (index >= 0 && index < cameras.size())
	{
		activeCameraIndex = index;
	}
}

const int Scene::GetActiveCameraIndex() const
{
	return activeCameraIndex;
}
// TODO
Camera* Scene::GetActiveCamera()
{
	
}
// TODO
void Scene::NextCamera()
{

}
// TODO
void Scene::DeleteActiveCamera()
{

}
// TODO
const bool Scene::ShouldRenderCamera(int cameraIndex)
{

}

void Scene::SetActiveModelIndex(int index)
{
	// implementation suggestion...
	if (index >= 0 && index < models.size())
	{
		activeModelIndex = index;
	}
}

const int Scene::GetActiveModelIndex() const
{
	return activeModelIndex;
}
// TODO
void Scene::SetWorldTransformation(const glm::mat4x4 world)
{

}
// TODO
const glm::mat4x4 Scene::GetWorldTransformation()
{

}
// TODO
const unsigned int Scene::AddPrimitiveModel(PRIM_MODEL primitiveModel)
{

}