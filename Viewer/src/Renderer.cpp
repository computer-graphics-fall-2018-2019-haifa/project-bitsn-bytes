#define _USE_MATH_DEFINES

#include "Renderer.h"
#include "InitShader.h"
#include "MeshModel.h"
#include "Utils.h"
#include "Constants.h"
#include <imgui/imgui.h>
#include <vector>
#include <cmath>

#define INDEX(width, x, y, c) ((x) + (y) * (width)) * 3 + (c)
#define IS_CAMERA true

Renderer::Renderer(int viewportWidth, int viewportHeight, int viewportX, int viewportY) :
	colorBuffer(nullptr),
	zBuffer(nullptr)
{
	initOpenGLRendering();
	SetViewport(viewportWidth, viewportHeight, viewportX, viewportY);
}

Renderer::~Renderer()
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}
}

void Renderer::createBuffers(int viewportWidth, int viewportHeight)
{
	if (colorBuffer)
	{
		delete[] colorBuffer;
	}

	colorBuffer = new float[3* viewportWidth * viewportHeight];
	for (int x = 0; x < viewportWidth; x++)
	{
		for (int y = 0; y < viewportHeight; y++)
		{
			PutPixel(x, y, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewportWidth; i++)
	{
		for (int j = 0; j < viewportHeight; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::SetViewport(int viewportWidth, int viewportHeight, int viewportX, int viewportY)
{
	this->viewportX = viewportX;
	this->viewportY = viewportY;
	this->viewportWidth = viewportWidth;
	this->viewportHeight = viewportHeight;
	createBuffers(viewportWidth, viewportHeight);
	createOpenGLBuffer();
}

void Renderer::Render(Scene& scene)
{
	if (scene.GetActiveCameraIndex() != DISABLED) {
		Camera* activeCamera = scene.GetActiveCamera();
		SetCameraTransformation(inverse(activeCamera->GetTransformation()));
		SetProjection(activeCamera->GetProjection());
	}

	DrawAxis(scene);

	std::vector<std::shared_ptr<MeshModel>> models = scene.GetModels();
	std::vector<Camera*> cameras = scene.GetCameras();

	for each (auto model in models)
	{
		const std::pair<std::vector<glm::vec3>, std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>>>* modelVertices;

		modelVertices = model->Render();
		SetObjectMatrices(model->GetModelTransformation(), model->GetNormalTransformation());
		SetWorldTransformation(scene.GetWorldTransformation());

		DrawTriangles(scene, &modelVertices->first, scene.ShouldShowFacesNormals(), &model->GetCentroid(), 1);


		if (scene.ShouldShowVerticesNormals() && !modelVertices->second.second.empty()) {
			DrawVerticesNormals(scene, modelVertices->second.first, modelVertices->second.second);
		}

		if (scene.ShouldShowBorderCube()) {
			DrawBorderCube(scene, model->GetBorderCube());
		}

		delete modelVertices;
	}

	for each(auto camera in cameras)
	{
		if (camera->IsModelRenderingActive() && camera != scene.GetActiveCamera()) {
			Camera* activeCamera = scene.GetActiveCamera();
			SetCameraTransformation(inverse(activeCamera->GetTransformation()));
			SetProjection(activeCamera->GetProjection());

			SetWorldTransformation(scene.GetWorldTransformation());

			glm::mat4x4 cameraTransformation = glm::mat4x4(SCALING_MATRIX4(1.f / 4.f)) * camera->GetTransformation();

			SetObjectMatrices(cameraTransformation, glm::mat4x4(I_MATRIX));

			const std::pair<std::vector<glm::vec3>, std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>>>* cameraVertices = camera->Render();

			DrawTriangles(scene, &cameraVertices->first, FALSE, NULL, 1, IS_CAMERA);

			delete cameraVertices;
		}
	}

	SwapBuffers();
}

void Renderer::DrawAxis(Scene& scene)
{
	glm::vec3 axisX		= { 1, 0, 0 };
	glm::vec3 axisY		= { 0, 1, 0 };
	glm::vec3 axisZ		= { 0, 0, 1 };
	glm::vec3 zeroPoint = { 0, 0, 0 };

	axisX = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * Utils::ToHomogeneousForm(axisX));
	axisY = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * Utils::ToHomogeneousForm(axisY));
	axisZ = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * Utils::ToHomogeneousForm(axisZ));
	zeroPoint = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * Utils::ToHomogeneousForm(zeroPoint));

	DrawLine(ToViewPlane(zeroPoint), ToViewPlane(axisX * 5.f), COLOR(X_COL));
	DrawLine(ToViewPlane(zeroPoint), ToViewPlane(axisY * 5.f), COLOR(Y_COL));
	DrawLine(ToViewPlane(zeroPoint), ToViewPlane(axisZ * 5.f), COLOR(YELLOW));
}

void Renderer::DrawLine(const glm::uvec2& p1, const glm::uvec2& p2, const glm::vec3& color)
{
	float dx, dy;

	float x1 = p1.x;
	float x2 = p2.x;
	float y1 = p1.y;
	float y2 = p2.y;

	const bool bStep = IsSlopeBiggerThanOne(x1, x2, y1, y2);

	OrderPoints(x1, x2, y1, y2);

	GetDeltas(x1, x2, y1, y2, &dx, &dy);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for (int x = (int)x1; x < maxX; x++)
	{
		PutPixel(x, y, bStep, color);

		yStepErrorUpdate(dx, dy, error, y, ystep);
	}
}

glm::uvec2 Renderer::ToViewPlane(const glm::vec2& point)
{
	glm::vec2 screenPoint;

	screenPoint.x = ((point.x + 1) * viewportWidth / 2.0f);
	screenPoint.y = ((point.y + 1) * viewportHeight / 2.0f);

	screenPoint.x = round((screenPoint.x - (viewportWidth / 2.0f)) * (500.0f / viewportWidth) + (viewportWidth / 2.0f));
	screenPoint.y = round((screenPoint.y - (viewportHeight / 2.0f)) * (500.0f / viewportHeight) + (viewportHeight / 2.0f));

	return glm::vec2(screenPoint.x, screenPoint.y);
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewportWidth) return;
	if (j < 0) return; if (j >= viewportHeight) return;
	colorBuffer[INDEX(viewportWidth, i, j, 0)] = color.x;
	colorBuffer[INDEX(viewportWidth, i, j, 1)] = color.y;
	colorBuffer[INDEX(viewportWidth, i, j, 2)] = color.z;
}

void Renderer::PutPixel(int x, int y, bool step, const glm::vec3& color)
{
	if (step)
	{
		PutPixel(y, x, color);
	}
	else
	{
		PutPixel(x, y, color);
	}
}

void Renderer::DrawTriangles(Scene& scene, const std::vector<glm::vec3>* vertices, bool shouldDrawFaceNormals /*= false*/, const glm::vec3* modelCentroid /*= NULL*/, UINT32 normScaleRate /*= 1*/, bool isCamera /*= false*/)
{
	std::vector<glm::vec3>::const_iterator it = vertices->begin();

	while (it != vertices->end())
	{
		glm::vec3 p1 = *(it++);
		if (it == vertices->end()) break;
		glm::vec3 p2 = *(it++);
		if (it == vertices->end()) break;
		glm::vec3 p3 = *(it++);

		glm::vec3 nrm1 = p1;
		glm::vec3 nrm2 = p2;
		glm::vec3 nrm3 = p3;

		p1 = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * objectTranformation * Utils::ToHomogeneousForm(p1));
		p2 = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * objectTranformation * Utils::ToHomogeneousForm(p2));
		p3 = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * objectTranformation * Utils::ToHomogeneousForm(p3));

		DrawLine(ToViewPlane(p1), ToViewPlane(p2), COLOR(WHITE));
		DrawLine(ToViewPlane(p2), ToViewPlane(p3), COLOR(WHITE));
		DrawLine(ToViewPlane(p3), ToViewPlane(p1), COLOR(WHITE));

		if (scene.ShouldShowFacesNormals())
		{
			glm::vec3 subs1 = nrm3 - nrm1;
			glm::vec3 subs2 = nrm2 - nrm1;
			glm::vec3 faceNormal = glm::cross(subs1, subs2);

			glm::vec3 faceCenter = (nrm1 + nrm2 + nrm3) / 3.0f;

			glm::vec3 normalizedFaceNormal = Utils::IsVecEqual(faceNormal, glm::vec3(0, 0, 0)) ? faceNormal : glm::normalize(faceNormal);

			normalizedFaceNormal /= 2.5f;
			glm::vec3 nP1 = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * objectTranformation * Utils::ToHomogeneousForm(faceCenter));
			glm::vec3 nP2 = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * objectTranformation * Utils::ToHomogeneousForm(faceCenter + normalizedFaceNormal));

			DrawLine(ToViewPlane(nP1), ToViewPlane(nP2), COLOR(LIME));
		}
	}
}

void Renderer::DrawVerticesNormals(Scene& scene, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals)
{
	for (int i = 0; i < normals.size() && i < vertices.size(); i++)
	{
		glm::vec3 vertex = vertices[i];
		glm::vec3 vertexNormal = normals[i];

		glm::vec3 nP1 = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * objectTranformation * Utils::ToHomogeneousForm(vertex));
		glm::vec3 nP2 = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * objectTranformation * Utils::ToHomogeneousForm(vertex + vertexNormal / 2.5f));

		DrawLine(ToViewPlane(nP1), ToViewPlane(nP2), COLOR(RED));
	}
}

void Renderer::DrawBorderCube(Scene& scene, CUBE_LINES& borderCube)
{
	for each (std::pair<glm::vec3, glm::vec3> line in borderCube.line)
	{
		glm::vec3 pStart = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * objectTranformation * Utils::ToHomogeneousForm(line.first));
		glm::vec3 pEnd = Utils::ToCartesianForm(scene.GetActiveCameraProjection() * scene.GetActiveCameraTransformation() * scene.GetWorldTransformation() * objectTranformation * Utils::ToHomogeneousForm(line.second));

		DrawLine(ToViewPlane(pStart), ToViewPlane(pEnd), COLOR(BLUE));
	}
}

void Renderer::OrderPoints(float& x1, float& x2, float& y1, float& y2)
{
	if (IsSlopeBiggerThanOne(x1, x2, y1, y2)) {
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
}

void Renderer::GetDeltas(IN float x1, IN float x2, IN float y1, IN float y2, OUT float* pDx, OUT float* pDy)
{
	*pDx = x2 - x1;
}

void Renderer::yStepErrorUpdate(float dx, float dy, float& error, int& y, const int& ystep)
{
	error -= dy;
	if (error < 0)
	{
		y += ystep;
		error += dx;
	}
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::initOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &glScreenTex);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &glScreenVtc);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(glScreenVtc);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewportWidth, viewportHeight, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewportWidth, viewportHeight);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewportWidth, viewportHeight, GL_RGB, GL_FLOAT, colorBuffer);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(glScreenVtc);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}