#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Scene.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

class Scene;

/*
 * Renderer class.
 */
class Renderer
{
private:
	float *colorBuffer;
	float *zBuffer;
	int viewportWidth;
	int viewportHeight;
	int viewportX;
	int viewportY;

	void createBuffers(int viewportWidth, int viewportHeight);

	GLuint glScreenTex;
	GLuint glScreenVtc;

	void createOpenGLBuffer();
	void initOpenGLRendering();

	glm::mat4x4 worldTranformation;
	glm::mat4x4 cameraTransformation;
	glm::mat4x4 objectTranformation;
	glm::mat4x4 normalTransformation;
	glm::mat4x4 projection;

	glm::uvec2 ToViewPlane(const glm::vec2& point);
	void OrderPoints(float& x1, float& x2, float& y1, float& y2);
	bool IsSlopeBiggerThanOne(float x1, float x2, float y1, float y2) { return (fabs(y2 - y1) > fabs(x2 - x1)); }

	void PutPixel(int x, int y, const glm::vec3& color);
	void PutPixel(int x, int y, bool steep, const glm::vec3& color);

	void GetDeltas(IN float x1, IN float x2, IN float y1, IN float y2, OUT float* pDx, OUT float* pDy);
	void yStepErrorUpdate(float dx, float dy, float& error, int& y, const int& ystep);

public:
	Renderer(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);
	~Renderer();

	void Render(Scene* scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	void SetViewport(int viewportWidth, int viewportHeight, int viewportX = 0, int viewportY = 0);

	void SetCameraTransformation(glm::mat4x4& cameraTransformation_) { cameraTransformation = cameraTransformation_; }
	void SetProjection(glm::mat4x4& projection_) { projection = projection_; }

	void SetObjectMatrices(const glm::mat4x4& objectTransformation_, const glm::mat4x4& normalTransformation_)
	{
		objectTranformation = objectTransformation_;
		normalTransformation = normalTransformation_;
	}
	void SetWorldTransformation(const glm::mat4x4& transformation) { worldTranformation = transformation; }

	void DrawAxis(Scene* scene);
	void DrawLine(const glm::uvec2& p1, const glm::uvec2& p2, const glm::vec3& color);
	void DrawTriangles(Scene* scene, const std::vector<glm::vec3>* triangles, bool shouldDrawFaceNormals = false, const glm::vec3* modelCentroid = NULL, UINT32 normScaleRate = 1, bool isCamera = false);
	void DrawVerticesNormals(Scene* scene, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals);
	void DrawBorderCube(Scene* scene, CUBE_LINES& cubeLines);
};

#endif // !__RENDERER_H__
