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
#include "Light.h"
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
		std::vector<Light*> lights;

		int activeCameraIndex;
		int activeModelIndex;
		int activeLightIndex;

		glm::mat4x4 worldTransformation;

		bool drawVerticesNormals;
		bool drawFacesNormals;
		bool drawBorderCube;
		bool drawWireframe;

		glm::vec4 polygonColor;
		glm::vec4 wireframeColor;
		glm::vec4 backgroundColor;

		float verticeNormalScaleFactor;
		float faceNormalScaleFactor;

		SHADING_TYPE shading;

	public:
		Scene();

		// Getters & Setters
		glm::vec4 GetPolygonColor();
		void SetvnScale(float scale);
		float GetvnScale();
		void SetfnScale(float scale);
		float GetfnScale();
		void SetGeneratedTexture();
		glm::vec4 GetBackgroundColor();
		void SetBackgroundColor(const glm::vec4& color);
		glm::vec4 GetPolygonColor();
		void SetPolygonColor(const glm::vec4& color);
		glm::vec4 GetWireframeColor();
		void SetWireframeColor(const glm::vec4& color);
		GENERATED_TEXTURE GetGeneratedTexture();
		void SetGeneratedTexture(GENERATED_TEXTURE texture);

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
		void TranslateModel(MeshModel* activeModel, AXIS axis, float value);
		void ScaleModel(MeshModel* activeModel, float value);
		void RotateModel(MeshModel* activeModel, AXIS axis, float angle);
		void RotateActiveModelRelativeToWorld(float angle, AXIS axis);
		void RotateModelRelativeToWorld(MeshModel* activeModel, AXIS axis, float angle);

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

		// Light related functions
		void AddLight(Light* light);
		Light* GetActiveLight();
		int GetActiveLightIndex();
		void SetActiveLightIndex(unsigned int index);
		void NextLight();
		void DeleteActiveLight();
		bool ShouldRenderLight();
		glm::mat4x4 GetActiveLightModelTransformation();

		// Actions
		void ShowVerticesNormals(const bool key);
		void ShowFacesNormals(const bool key);
		void ShowBorderCube(const bool key);
		bool ShouldShowVerticesNormals() { return drawVerticesNormals; }
		bool ShouldShowFacesNormals() { return drawFacesNormals; }
		bool ShouldShowBorderCube() { return drawBorderCube; }
		void DrawWireframe(const bool flag);
		void SetShadingType(SHADING_TYPE type);

		// Projection functions
		void SetOrthographicProjection(const PROJECTION_PARAMETERS);
		void SetPerspectiveProjection(const PERSPECTIVE_PARAMETERS);
		void SetFrustumViewVolume(const PROJECTION_PARAMETERS);

		// Scale function
		void ScaleActiveCamera(const float scaleFactor);
		void ScaleActiveModel(const float scaleFactor);
		void ScaleActiveLightModel(const float scaleFactor);

		// Translation functions
		void TranslateActiveCameraAxis(float moveFactor, AXIS axis);
		void TranslateActiveModel(float moveFactor, AXIS axis);
		void TranslateActiveLight(float moveFactor, AXIS axis);

		// Rotation functions
		void RotateActiveCameraRelativeToWorld(float angle, AXIS axis);
		void RotateActiveCamera(float angle, AXIS axis);
		void RotateActiveModelRelativeToWorld(float angle, AXIS axis);
		void RotateActiveModel(float angle, AXIS axis);
		void RotateActiveLightModelRelativeToWorld(float angle, AXIS axis);
		void RotateActiveLightModel(float angle, AXIS axis);

		// Transformation related functions
		void SetWorldTransformation(const glm::mat4x4 world);
		const glm::mat4x4 GetWorldTransformation();
};

#endif // !__SCENE_H__