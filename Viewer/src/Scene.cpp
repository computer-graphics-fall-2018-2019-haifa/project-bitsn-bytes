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
	SetActiveModelIndex(models.size() - 1);
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

void Scene::AddPrimitiveModel(PRIMITIVE primitiveModel)
{
	std::shared_ptr<MeshModel> model = std::make_shared<MeshModel>(PrimMeshModel(primitiveModel));
	models.push_back(model);
	SetActiveModelIndex(models.size() - 1);
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
	camera->SetCameraIndex(GetCameraCount() - 1);
	SetActiveCameraIndex(cameras.size() - 1);
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
		return cameras.at(activeCameraIndex);
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
	if (cameraIndex != DISABLED) {
		return GetActiveCamera()->IsModelRenderingActive();
	}
}

void Scene::SetOrthographicProjection(const PROJECTION_PARAMETERS parameters)
{
	if (activeCameraIndex != DISABLED) {
		GetActiveCamera()->SetOrthographicProjection(parameters);
	}
}

void Scene::SetPerspectiveProjection(const PERSPECTIVE_PARAMETERS parameters)
{
	if (activeCameraIndex != DISABLED) {
		GetActiveCamera()->SetPerspectiveProjection(parameters);
	}
}

void Scene::SetFrustumViewVolume(const PROJECTION_PARAMETERS parameters)
{
	if (activeCameraIndex != DISABLED) {
		GetActiveCamera()->SetFrustumViewVolume(parameters);
	}
}

const glm::mat4x4 Scene::GetActiveCameraTransformation()
{
	if (activeCameraIndex != DISABLED) {
		return GetActiveCamera()->GetTransformation();
	}
	else {
		return I_MATRIX;
	}
}

const glm::mat4x4 Scene::GetActiveCameraProjection()
{
	if (activeCameraIndex != DISABLED) {
		return GetActiveCamera()->GetProjection();
	}
	else {
		return I_MATRIX;
	}
}

void Scene::ScaleActiveCamera(const float scaleFactor)
{
	if (activeCameraIndex != DISABLED) {
		Camera* activeCamera = GetActiveCamera();
		glm::mat4x4 currentTransformation = activeCamera->GetTransformation();
		glm::mat4x4 scaleTransformation(SCALING_MATRIX4(scaleFactor));
		activeCamera->SetTransformation(scaleTransformation * currentTransformation);
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

glm::vec4 Scene::GetBackgroundColor()
{
	return backgroundColor;
}

void Scene::SetBackgroundColor(const glm::vec4& color)
{
	backgroundColor = color;
}

glm::vec4 Scene::GetPolygonColor()
{
	return polygonColor;
}

void Scene::SetPolygonColor(const glm::vec4& color)
{
	polygonColor = color;
}

glm::vec4 Scene::GetWireframeColor()
{
	return wireframeColor;
}

void Scene::SetWireframeColor(const glm::vec4& color)
{
	wireframeColor = color;
}

GENERATED_TEXTURE Scene::GetGeneratedTexture()
{
	return renderer->GetGeneratedTexture();
}

void Scene::SetGeneratedTexture(GENERATED_TEXTURE texture)
{
	renderer->SetGeneratedTexture(texture);
}

int Scene::GetActiveLightIndex()
{
	return activeLightIndex;
}

void Scene::SetActiveLightIndex(unsigned int index)
{
	if (lights.size() > index)
	{
		activeLightIndex = index;
	}
}

void Scene::NextLight()
{
	if (activeLightIndex != DISABLED)
	{
		activeLightIndex = (activeLightIndex + 1) % lights.size();
	}
}

void Scene::DeleteActiveLight()
{
	if (activeLightIndex != DISABLED)
	{
		lights.erase(lights.begin() + activeLightIndex);
		activeLightIndex = (unsigned)lights.size() - 1;
	}
}

glm::mat4x4 Scene::GetActiveLightModelTransformation()
{
	if (activeLightIndex != DISABLED)
	{
		MeshModel* activeLightModel = &lights[activeLightIndex]->GetLightModel();
		return activeLightModel->GetModelTransformation();
	}
	else
	{
		return ZERO_MATRIX;
	}
}

void Scene::TranslateActiveLight(float value, AXIS axis)
{
	if (activeLightIndex != DISABLED)
	{
		MeshModel* activeLightModel = &lights[activeLightIndex]->GetLightModel();
		TranslateModel(activeLightModel, axis, value);
	}
}

void Scene::ScaleActiveLightModel(float value)
{
	if (activeLightIndex != DISABLED)
	{
		MeshModel* activeLightModel = &lights[activeLightIndex]->GetLightModel();
		ScaleModel(activeLightModel, value);
	}
}

void Scene::RotateActiveLightModel(float angle, AXIS axis)
{
	if (activeLightIndex != DISABLED)
	{
		MeshModel* activeLightModel = &lights[activeLightIndex]->GetLightModel();
		RotateModel(activeLightModel, axis, angle);
	}
}

void Scene::RotateActiveLightModelRelativeToWorld(float angle, AXIS axis)
{
	if (activeLightIndex != DISABLED)
	{
		MeshModel* activeLightModel = &lights[activeLightIndex]->GetLightModel();
		RotateModelRelativeToWorld(activeLightModel, axis, angle);
	}
}

bool Scene::ShouldRenderLight()
{
	if (activeLightIndex != DISABLED)
	{
		return lights[activeLightIndex]->IsModelRenderingActive();
	}

	else return false;
}

void Scene::AddLight(Light* light)
{
	Light* p_newLight = nullptr;

	switch (type)
	{
	case LIGHT_SOURCE_TYPE::POINT:
		p_newLight = new PointSourceLight(lightCoord, ambiantC, ambiantI, diffusiveC, diffusiveI, specularC, specularI);
		break;
	case LIGHT_SOURCE_TYPE::PARALLEL:
		p_newLight = new ParallelSourceLight(lightCoord, ambiantC, ambiantI, diffusiveC, diffusiveI, specularC, specularI);
		break;
	case LIGHT_SOURCE_TYPE::AREA:
		p_newLight = new DistributedSourceLight(lightCoord, ambiantC, ambiantI, diffusiveC, diffusiveI, specularC, specularI);
		break;

	default:
		break;
	}

	lights.push_back(p_newLight);
}


Light* Scene::GetActiveLight()
{
	if (activeLightIndex != DISABLED) {
		return lights[activeLightIndex];
	}
	else return nullptr;
}


void Scene::SetShadingType(SHADING_TYPE shading)
{
	shading = shading;
}

void Scene::DrawWireframe(bool bDrawn)
{
	drawWireframe = bDrawn;
}

float Scene::GetvnScale()
{
	return verticeNormalScaleFactor;
}

void Scene::SetvnScale(float scale)
{
	verticeNormalScaleFactor = scale;
}

float Scene::GetfnScale()
{
	return faceNormalScaleFactor;
}

void Scene::SetfnScale(float scale)
{
	faceNormalScaleFactor = scale;
}