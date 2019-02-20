#pragma once
#define _USE_MATH_DEFINES

#include "ImguiMenus.h"
#include "MeshModel.h"
#include "Utils.h"
#include "Constants.h"
#include <cmath>
#include <memory>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <nfd.h>
#include <random>
#include <GLFW/glfw3.h>

bool showDemoWindow = false;
bool showAnotherWindow = false;

bool modelControlWindow = false;
static bool modelControlFrame = false;

bool lightingMenu = true;
static bool lightControlFrame = false;

bool colorMenu = false;
bool showFile = false;

glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
Surface material;

const glm::vec4& GetClearColor()
{
	return clearColor;
}

void DrawImguiMenus(ImGuiIO& io, Scene* scene)
{
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		ImGui::Begin("Main menu");
		static int world[4] = { 1,1,1 };
		ImGui::InputInt3("World transformation: (x,y,z)", world);
		ImGui::Text("World transformation: (%d, %d, %d)", world[0], world[1], world[2]);
		if (ImGui::Button("Set world transformation"))
		{
			scene->SetWorldTransformation(HOMOGENEOUS_MATRIX4(world[0], world[1], world[2], 1.0f));
		}
		static glm::mat4x4 worldTransformation = glm::mat4x4(0);
		worldTransformation = scene->GetWorldTransformation();
		std::string sWorldTransform = "";
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				sWorldTransform.append(std::to_string(worldTransformation[i][j]) + " ");
			}
			sWorldTransform.append("\n");
		}
		ImGui::Text("World transformation:\n");
		ImGui::Text(sWorldTransform.c_str());

		//cene->updateCurrentDims((int) ImGui::GetWindowHeight(), (int) ImGui::GetWindowWidth());



		ImGui::Text("------------------- Cameras: -------------------");

		static float eye[3] = { 2,2,2 };
		static float at[3] = { 0,0,0 };
		ImGui::SliderFloat3("Look from: (x,y,z)", eye, -10, 10);
		ImGui::SliderFloat3("Look at: (x,y,z)", at, -10, 10);
		ImGui::Text("Look from: (%d, %d, %d)", eye[0], eye[1], eye[2]);
		ImGui::Text("Look at: (%d, %d, %d)", at[0], at[1], at[2]);
		if (ImGui::Button("Add new camera"))
		{
			Camera* camera = new Camera({ eye[0], eye[1], eye[2], 1.0f }, { at[0], at[1], at[2], 1.0f }, { 0, 1, 0, 1.0f });
			scene->AddCamera(camera);
		}
		ImGui::SameLine();

		if (ImGui::Button("Next camera"))
		{
			scene->NextCamera();
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete camera"))
		{
			scene->DeleteActiveCamera();
		}
		//         ImGui::SameLine();
		//         if (ImGui::Button("Apply to camera (todo)"))
		//         {
		//             
		//         }

		static bool showCamera = false;
		int cameraIndex = scene->GetActiveCameraIndex();
		ImGui::Text("Active camera: %d", cameraIndex);
		showCamera = scene->ShouldRenderCamera(cameraIndex);
		ImGui::SameLine();
		if (ImGui::Checkbox("Show Camera", &showCamera))
		{
			if (scene->GetActiveCamera() != NULL) {
				scene->GetActiveCamera()->SetModelRenderingState(showCamera);
			}
		}


		static glm::mat4x4 activeCameraTransformation = glm::mat4x4(0);
		activeCameraTransformation = scene->GetActiveCameraTransformation();
		std::string sCameraTransform = "";
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				sCameraTransform.append(std::to_string(activeCameraTransformation[i][j]) + " ");
			}
			sCameraTransform.append("\n");
		}

		ImGui::Text("Camera transformation:\n");
		ImGui::Text(sCameraTransform.c_str());


		ImGui::Text("---------------- Camera Control: ----------------");


		static int currentFrame = FRAME_TYPE::CAMERA_;
		static AXIS currentAxis = AXIS::Y;
		static int currentRel = ROTATION_REL::WORLD;

		static std::string sCurrentFrame = "Camera";
		static std::string sCurrentAxis = "Y-axis";
		static std::string sCurrentRel = "World";

		ImGui::TextColored({ 1.f, 0.4f, 0.4f, 1.f }, (sCurrentFrame + " relative to " + sCurrentAxis + " of " + sCurrentRel).c_str());

		ImGui::Text("Frame: ");
		ImGui::SameLine();
		if (ImGui::SmallButton("World"))
		{
			sCurrentFrame = "World";
			currentFrame = FRAME_TYPE::WORLD_;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Camera"))
		{
			sCurrentFrame = "Camera";
			currentFrame = FRAME_TYPE::CAMERA_;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Model"))
		{
			sCurrentFrame = "Model";
			currentFrame = FRAME_TYPE::MODEL_;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Light"))
		{
			sCurrentFrame = "Light";
			currentFrame = FRAME_TYPE::LIGHT_;
		}

		ImGui::Text("Relative to: ");
		ImGui::SameLine();
		if (ImGui::SmallButton("itself"))
		{
			sCurrentRel = "itself";
			currentRel = ROTATION_REL::SELF;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("world"))
		{
			sCurrentRel = "World";
			currentRel = ROTATION_REL::WORLD;
		}

		ImGui::Text("Axis: ");
		ImGui::SameLine();
		if (ImGui::SmallButton("X-axis"))
		{
			sCurrentAxis = "X-axis";
			currentAxis = AXIS::X;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Y-axis"))
		{
			sCurrentAxis = "Y-axis";
			currentAxis = AXIS::Y;
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("Z-axis"))
		{
			sCurrentAxis = "Z-axis";
			currentAxis = AXIS::Z;
		}


		ImGui::Text("Rotate:");
		static auto rotAngle = static_cast<float>(PI / 36.0f);
		ImGui::SliderAngle("rotation angle", &rotAngle, 1.0f, 180.0f);

		if (!ImGui::IsMouseHoveringAnyWindow() && io.MouseDown[0] && io.MouseDelta.x)
		{
			auto direction = static_cast<int>(io.MouseDelta.x / abs(io.MouseDelta.x));

			switch (currentFrame)
			{
			case FRAME_TYPE::CAMERA_:
			{
				switch (currentRel)
				{
				case ROTATION_REL::WORLD:
				{
					scene->RotateActiveCameraRelativeToWorld(rotAngle * direction, currentAxis);
				} break;
				case ROTATION_REL::SELF:
				{
					scene->RotateActiveCamera(rotAngle * direction, currentAxis);
				} break;
				default: break;
				}
			} break;
			case FRAME_TYPE::MODEL_:
			{
				switch (currentRel)
				{
				case ROTATION_REL::WORLD:
				{
					scene->RotateActiveModelRelativeToWorld(rotAngle * direction, currentAxis);
				} break;
				case ROTATION_REL::SELF:
				{
					scene->RotateActiveModel(rotAngle * direction, currentAxis);
				} break;
				default: break;
				}
			} break;
			case FRAME_TYPE::LIGHT_:
			{
				switch (currentRel)
				{
				case ROTATION_REL::WORLD:
				{
					scene->RotateActiveLightModelRelativeToWorld(rotAngle * direction, currentAxis);
				} break;
				case ROTATION_REL::SELF:
				{
					scene->RotateActiveLightModel(rotAngle * direction, currentAxis);
				} break;
				default: break;
				}
			} break;
			default: break;
			}
		}

		ImGui::Text("Zoom:");
		//Camera scaling:            
		static float camScaleFactor = 1.5f;
		ImGui::SliderFloat("scaling factor", &camScaleFactor, 1.0f, 10.0f);

		if (ImGui::IsKeyPressed('=') || io.MouseWheel > 0 || ImGui::Button("   Zoom in   "))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
		{
			switch (currentFrame)
			{
			case FRAME_TYPE::CAMERA_:
			{
				scene->ScaleActiveCamera(1.f / camScaleFactor);
			} break;
			case FRAME_TYPE::MODEL_:
			{
				scene->ScaleActiveModel(camScaleFactor);
			} break;
			case FRAME_TYPE::LIGHT_:
			{
				scene->ScaleActiveLightModel(1.0f / camScaleFactor);
			} break;
			default: break;
			}

		}
		ImGui::SameLine();
		if (ImGui::IsKeyPressed('-') || io.MouseWheel < 0 || ImGui::Button("   Zoom out   "))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
		{
			switch (currentFrame)
			{
			case FRAME_TYPE::CAMERA_:
			{
				scene->ScaleActiveCamera(camScaleFactor);
			} break;
			case FRAME_TYPE::MODEL_:
			{
				scene->ScaleActiveModel(1.0f / camScaleFactor);
			} break;
			case FRAME_TYPE::LIGHT_:
			{
				scene->ScaleActiveLightModel(camScaleFactor);
			} break;
			default: break;
			}

		}

		//Camera moves:
		ImGui::Text("Move:");
		static float moveFactor = 0.5f;
		ImGui::SliderFloat("move factor", &moveFactor, 0.1f, 1.f);

		if (ImGui::IsKeyPressed(GLFW_KEY_LEFT) || ImGui::Button("  Left  "))
		{
			switch (currentFrame)
			{
			case FRAME_TYPE::CAMERA_:
			{
				scene->TranslateActiveCameraAxis(-moveFactor, AXIS::X);
			} break;
			case FRAME_TYPE::MODEL_:
			{
				scene->TranslateActiveModel(-moveFactor, AXIS::X);
			} break;
			case FRAME_TYPE::LIGHT_:
			{
				scene->TranslateActiveLight(-moveFactor, AXIS::X);
			} break;
			default: break;
			}
		}
		ImGui::SameLine();
		if (ImGui::IsKeyPressed(GLFW_KEY_RIGHT) || ImGui::Button("  Right "))
		{
			switch (currentFrame)
			{
			case FRAME_TYPE::CAMERA_:
			{
				scene->TranslateActiveCameraAxis(moveFactor, AXIS::X);
			} break;
			case FRAME_TYPE::MODEL_:
			{
				scene->TranslateActiveModel(moveFactor, AXIS::X);
			} break;
			case FRAME_TYPE::LIGHT_:
			{
				scene->TranslateActiveLight(moveFactor, AXIS::X);
			} break;
			default: break;
			}
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_UP) || ImGui::Button("   Up   "))
		{
			switch (currentFrame)
			{
			case FRAME_TYPE::CAMERA_:
			{
				scene->TranslateActiveCameraAxis(moveFactor, AXIS::Y);
			} break;
			case FRAME_TYPE::MODEL_:
			{
				scene->TranslateActiveModel(moveFactor, AXIS::Y);
			} break;
			case FRAME_TYPE::LIGHT_:
			{
				scene->TranslateActiveLight(moveFactor, AXIS::Y);
			} break;
			default: break;
			}
		}
		ImGui::SameLine();
		if (ImGui::IsKeyPressed(GLFW_KEY_DOWN) || ImGui::Button("  Down  "))
		{
			switch (currentFrame)
			{
			case FRAME_TYPE::CAMERA_:
			{
				scene->TranslateActiveCameraAxis(-moveFactor, AXIS::Y);
			} break;
			case FRAME_TYPE::MODEL_:
			{
				scene->TranslateActiveModel(-moveFactor, AXIS::Y);
			} break;
			case FRAME_TYPE::LIGHT_:
			{
				scene->TranslateActiveLight(-moveFactor, AXIS::Y);
			} break;
			default: break;
			}
		}
		if (ImGui::IsKeyPressed(GLFW_KEY_PAGE_UP) || ImGui::Button(" Forward"))
		{
			switch (currentFrame)
			{
			case FRAME_TYPE::CAMERA_:
			{
				scene->TranslateActiveCameraAxis(moveFactor, AXIS::Z);
			} break;
			case FRAME_TYPE::MODEL_:
			{
				scene->TranslateActiveModel(moveFactor, AXIS::Z);
			} break;
			case FRAME_TYPE::LIGHT_:
			{
				scene->TranslateActiveLight(moveFactor, AXIS::Z);
			} break;
			default: break;
			}
		}
		ImGui::SameLine();
		if (ImGui::IsKeyPressed(GLFW_KEY_PAGE_DOWN) || ImGui::Button("  Back  "))
		{
			switch (currentFrame)
			{
			case FRAME_TYPE::CAMERA_:
			{
				scene->TranslateActiveCameraAxis(-moveFactor, AXIS::Z);
			} break;
			case FRAME_TYPE::MODEL_:
			{
				scene->TranslateActiveModel(-moveFactor, AXIS::Z);
			} break;
			case FRAME_TYPE::LIGHT_:
			{
				scene->TranslateActiveLight(-moveFactor, AXIS::Z);
			} break;
			default: break;
			}
		}

		////////////////////// PROJECTION //////////////////////////////
		ImGui::Text("Projection:");

		static PROJECTION_PARAMETERS projectionParams =
		{
			projectionParams.left = -1,
			projectionParams.right = 1,
			projectionParams.bottom = -1,
			projectionParams.top = 1,
			projectionParams.zNear = 0.1f,
			projectionParams.zFar = 2,
		};
		ImGui::SliderFloat("Left", &projectionParams.left, -5.0, 5.0);
		ImGui::SliderFloat("Right", &projectionParams.right, -5.0, 5.0);
		ImGui::SliderFloat("Bottom", &projectionParams.bottom, -5.0, 5.0);
		ImGui::SliderFloat("Top", &projectionParams.top, -5.0, 5.0);
		ImGui::SliderFloat("zNear", &projectionParams.zNear, -5.0, 5.0);
		ImGui::SliderFloat("zFar", &projectionParams.zFar, -5.0, 5.0);

		static bool bIsProjError = false;
		static bool bIsPersError = false;


		try
		{
			if (ImGui::Button("Orthographic Projection"))
			{
				scene->SetOrthographicProjection(projectionParams);
			}
		}
		catch (bool thrownErrorState)
		{
			bIsProjError = thrownErrorState;
		}

		static bool bisFirst = true;

		if (bIsProjError)
		{
			ImGui::Text("Invalid input parameters, can't divide by zero - Try Again!");
			bIsPersError = false;
		}

		static PERSPECTIVE_PARAMETERS perspectiveParams =
		{
			perspectiveParams.fovy = static_cast<float>(TO_RADIAN(325)),
			perspectiveParams.aspect = 1/*ImGui::GetWindowWidth() / ImGui::GetWindowHeight()*/,
			perspectiveParams.zNear = 0.5f,
			perspectiveParams.zFar = 2.f
		};
		ImGui::SliderAngle("FOV", &perspectiveParams.fovy, 1, 350);
		ImGui::SliderFloat("Near", &perspectiveParams.zNear, 0.1f, 10.f);
		ImGui::SliderFloat("Far", &perspectiveParams.zFar, 0.2f, 20.f);

		try
		{
			if (ImGui::Button("Perspective Projection"))
			{
				scene->SetPerspectiveProjection(perspectiveParams);
			}
		}
		catch (bool thrownErrorState)
		{
			bIsPersError = thrownErrorState;
		}

		if (bIsPersError)
		{
			ImGui::Text("Invalid input parameters, can't divide by zero - Try Again!");
			bIsProjError = false;
		}

		static glm::mat4x4 activeCameraProjection;
		activeCameraProjection = scene->GetActiveCameraProjection();
		std::string sCameraProjection = "";
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				sCameraProjection.append(std::to_string(activeCameraProjection[i][j]) + " ");
			}
			sCameraProjection.append("\n");
		}

		ImGui::Text("Camera Projection:\n");
		ImGui::Text(sCameraProjection.c_str());

		ImGui::End();
	}

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	if (modelControlWindow)
	{
		ImGui::Begin("Models");
		ImGui::Text("-------------- Primitives Models: --------------");

		ImGui::Text("Material:");

		static int currentTexture = static_cast<int>(GENERATED_TEXTURE::NO_TEXTURE);
		static const char TextureTypes[] = { "None\0Crystal\0Rug\0" };
		ImGui::Text("Generated Non-Uniform Texture");
		bool static colorsNotInited = false;
		if (ImGui::Combo("GT", &currentTexture, TextureTypes))
		{
			colorsNotInited = true;
		}
		scene->SetGeneratedTexture(static_cast<GENERATED_TEXTURE>(currentTexture));

		glm::vec4 currentPolygonCol = scene->GetPolygonColor();
		static float ambientMatColor[3] = { currentPolygonCol.x + 0.1 ,currentPolygonCol.y + 0.1 ,currentPolygonCol.z + 0.1 };
		static float ambiantMatCoef = 1.f;
		ImGui::Text("Ambient Reflection Color");
		ImGui::ColorEdit3("ARC", ambientMatColor);
		ImGui::Text("Ambient Reflection Rate");
		ImGui::SliderFloat("ARR", &ambiantMatCoef, 0.f, 2.f);

		static float diffusiveMatColor[3] = { currentPolygonCol.x + 0.2 ,currentPolygonCol.y + 0.2 ,currentPolygonCol.z + 0.2 };
		static float diffusiveMatCoef = 1.f;
		ImGui::Text("Diffusive Reflection Color");
		ImGui::ColorEdit3("DRC", diffusiveMatColor);
		ImGui::Text("Diffusive Reflection Rate");
		ImGui::SliderFloat("DRR", &diffusiveMatCoef, 0.f, 2.f);

		static float specularMatColor[3] = { currentPolygonCol.x + 0.3 ,currentPolygonCol.y + 0.3 ,currentPolygonCol.z + 0.3 };
		static float specularMatCoef = 1.f;
		ImGui::Text("Specular Reflection Color");
		ImGui::ColorEdit3("SRC", specularMatColor);
		ImGui::Text("Specular Reflection Rate");
		ImGui::SliderFloat("SRR", &specularMatCoef, 0.f, 2.f);
		ImGui::NewLine();
		static int shininess = 1;
		ImGui::SliderInt("Shininess", &shininess, 1, 128);

		std::string sMaterial = "Custom";

		if (colorsNotInited)
		{
			switch (static_cast<GENERATED_TEXTURE>(currentTexture))
			{
			case GENERATED_TEXTURE::CRYSTAL:
			{
				sMaterial = "Crystal";
				ambientMatColor[0] = 0;
				ambientMatColor[1] = 1;
				ambientMatColor[2] = 0;
				ambiantMatCoef = 1.45f;
				diffusiveMatColor[0] = 1;
				diffusiveMatColor[1] = 0;
				diffusiveMatColor[2] = 0;
				diffusiveMatCoef = 0.85f;
				specularMatColor[0] = 0;
				specularMatColor[1] = 0;
				specularMatColor[2] = 1;
				specularMatCoef = 2.f;
				shininess = 32;

				colorsNotInited = false;
			} break;

			case GENERATED_TEXTURE::RUG:
			{
				sMaterial = "Rug";
				ambientMatColor[0] = 158.f / 255.f;
				ambientMatColor[1] = 147.f / 255.f;
				ambientMatColor[2] = 69.f / 255.f;
				ambiantMatCoef = 0.1f;
				diffusiveMatColor[0] = 171.f / 255.f;
				diffusiveMatColor[1] = 157.f / 255.f;
				diffusiveMatColor[2] = 101.f / 255.f;
				diffusiveMatCoef = 0.5f;
				specularMatColor[0] = 244.f / 255.f;
				specularMatColor[1] = 250.f / 255.f;
				specularMatColor[2] = 188.f / 255.f;
				specularMatCoef = 0.3f;
				shininess = 1;

				colorsNotInited = false;
			} break;

			default:

				break;

			}
		}


		material = Surface(sMaterial,
			{ ambientMatColor[0], ambientMatColor[1], ambientMatColor[2], 1.f },
			ambiantMatCoef,
			{ diffusiveMatColor[0], diffusiveMatColor[1], diffusiveMatColor[2], 1.f },
			diffusiveMatCoef,
			{ specularMatColor[0], specularMatColor[1], specularMatColor[2], 1.f },
			specularMatCoef,
			2 * shininess);


		if (ImGui::Button("Add Cube model"))
		{
			scene->AddPrimitiveModel(PRIMITIVE::CUBE);
			modelControlWindow = true;
		}

		if (ImGui::Button("Add Sphere model"))
		{
			scene->AddPrimitiveModel(PRIMITIVE::SPHERE);
			modelControlWindow = true;
		}

		if (ImGui::Button("Add Teapot model"))
		{
			scene->AddPrimitiveModel(PRIMITIVE::TEAPUT);
			modelControlWindow = true;
		}

		if (modelControlFrame)
		{
			ImGui::Text("-------------- Models Control: --------------");

			if (ImGui::Button("Next model"))
			{
				scene->NextModel();
			}
			if (ImGui::Button("Delete model"))
			{
				scene->DeleteActiveModel();
				if (scene->GetActiveModelIndex() == DISABLED)
				{
					modelControlFrame = false;
				}
			}
			ImGui::Text("Active model: %d", scene->GetActiveModelIndex());

			static glm::mat4x4 activeModelTransformation;
			activeModelTransformation = scene->GetActiveModelTransformation();
			std::string sModelTransformation = "";
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					sModelTransformation.append(std::to_string(activeModelTransformation[i][j]) + " ");
				}
				sModelTransformation.append("\n");
			}

			ImGui::Text("Model transformation:\n");
			ImGui::Text(sModelTransformation.c_str());

			static float normals[2] = { 0.1f, 0.1f };
			ImGui::SliderFloat2("vn/fn", normals, 0.1f, 2.0f);

			scene->SetvnScale(normals[0]);
			scene->SetfnScale(normals[1]);

			bool        bShowVertNorms = normals[0] > 0.1f ? true : false;
			bool        bShowFaceNorms = normals[1] > 0.1f ? true : false;
			static bool bShowBorderCube = false;

			scene->ShowVerticesNormals(bShowVertNorms);
			scene->ShowFacesNormals(bShowFaceNorms);
			scene->ShowBorderCube(bShowBorderCube);

			ImGui::Checkbox("Show vertices normals", &bShowVertNorms);
			ImGui::Checkbox("Show face normals", &bShowFaceNorms);
			ImGui::Checkbox("Show Border Cube", &bShowBorderCube);


		}

		ImGui::End();
	}

	if (lightingMenu)
	{
		ImGui::Begin("Lighting & Shading");

		ImGui::Text("-------------- Shading: --------------");

		static bool bDrawWireframe = false;
		if (ImGui::RadioButton("Show Wireframe", bDrawWireframe))
		{
			bDrawWireframe = bDrawWireframe ? false : true;
		}
		scene->DrawWireframe(bDrawWireframe);

		static int currentShading = static_cast<int>(SHADING_TYPE::GOURAUD);
		static const char ShadingTypes[] = { "None\0Solid\0Flat\0Phong\0Gouraud\0" };
		ImGui::Text("Shading Type");
		ImGui::Combo("ST", &currentShading, ShadingTypes);

		scene->SetShadingType(static_cast<SHADING_TYPE>(currentShading));

		static int currentLight = static_cast<int>(LIGHT_SOURCE_TYPE::POINT_);
		static const char LightTypes[] = { "Point\0Parallel\0Area\0" };
		ImGui::Text("Light Source");
		ImGui::Combo("LS", &currentLight, LightTypes);

		glm::vec4 currentPolygonCol = scene->GetPolygonColor();
		static float ambientColor[3] = { 1, 1, 1 };
		static float ambiantIntensity = 0.1f;
		ImGui::Text("Ambient Color");
		ImGui::ColorEdit3("AC", ambientColor);
		ImGui::Text("Ambient Intensity");
		ImGui::SliderFloat("AI: ", &ambiantIntensity, 0, 2);

		static float diffusiveColor[3] = { 1, 1, 1 };
		static float diffusiveIntensity = 0.3f;
		ImGui::Text("Diffusive Color");
		ImGui::ColorEdit3("DC", diffusiveColor);
		ImGui::Text("Diffusive Intensity");
		ImGui::SliderFloat("DI", &diffusiveIntensity, 0, 2);

		static float specularColor[3] = { 1 ,1 ,1 };
		static float specularIntensity = 0.35f;
		ImGui::Text("Specular Color");
		ImGui::ColorEdit3("SC", specularColor);
		ImGui::Text("Specular Intensity");
		ImGui::SliderFloat("SI", &specularIntensity, 0, 2);

		ImGui::Text("Light Location");
		static float lightCoord[3] = { 3.f , 3.f , 3.f };
		ImGui::SliderFloat3("LL", lightCoord, -10, 10);

		if (ImGui::Button("Add new light"))
		{
			LIGHT_SOURCE_TYPE type = static_cast<LIGHT_SOURCE_TYPE>(currentLight);
			Light* light = NULL;

			glm::vec3 coordinates = { lightCoord[0], lightCoord[2], lightCoord[3] };
			glm::vec4 ambiantC = { ambientColor[0], ambientColor[1], ambientColor[2], 1 };
			glm::vec4 diffusiveC = { diffusiveColor[0], diffusiveColor[1], diffusiveColor[2], 1 };
			glm::vec4 specularC = { specularColor[0], specularColor[1], specularColor[2], 1 };

			switch (type)
			{
				case LIGHT_SOURCE_TYPE::POINT_:
					light = new PointSourceLight(coordinates, ambiantC, ambiantIntensity, diffusiveC, diffusiveIntensity, specularC, specularIntensity);
					break;
				case LIGHT_SOURCE_TYPE::PARALLEL:
					light = new ParallelSourceLight(coordinates, ambiantC, ambiantIntensity, diffusiveC, diffusiveIntensity, specularC, specularIntensity);
					break;
				case LIGHT_SOURCE_TYPE::AREA:
					light = new DistributedSourceLight(coordinates, ambiantC, ambiantIntensity, diffusiveC, diffusiveIntensity, specularC, specularIntensity);
					break;

				default:
					break;
			}

			lightControlFrame = true;
		}


		if (lightControlFrame)
		{
			ImGui::Text("-------------- Active Light: --------------");

			Light* activeLight = scene->GetActiveLight();
			MeshModel* activeLightModel = activeLight ? &activeLight->GetLightModel() : nullptr;

			if (activeLightModel)
			{
				static bool bShowLight = false;
				bShowLight = activeLightModel->IsModelRenderingActive();


				if (ImGui::Checkbox("Show active light model", &bShowLight))
				{
					activeLightModel->SetModelRenderingState(bShowLight);
				}
			}

			if (activeLight)
			{
				float ambCol[3] = { activeLight->GetAmbientColor().x, activeLight->GetAmbientColor().y, activeLight->GetAmbientColor().z };
				float ambI = activeLight->GetAmbientIntensity();
				ImGui::Text("Active Ambient Color");
				ImGui::ColorEdit3("AAC", ambCol);
				ImGui::Text("Active Ambient Intensity");
				ImGui::SliderFloat("AAI", &ambI, 0, 2);
				activeLight->SetAmbientColor({ ambCol[0], ambCol[1], ambCol[2], 1 });
				activeLight->SetAmbientIntensity(ambI);


				float diffCol[3] = { activeLight->GetDiffusiveColor().x, activeLight->GetDiffusiveColor().y, activeLight->GetDiffusiveColor().z };
				float diffI = activeLight->GetDiffusiveIntensity();
				ImGui::Text("Active Diffusive Color");
				ImGui::ColorEdit3("ADC", diffCol);
				ImGui::Text("Active Diffusive Intensity");
				ImGui::SliderFloat("ADI", &diffI, 0, 2);
				activeLight->SetDiffusiveColor({ diffCol[0], diffCol[1], diffCol[2], 1 });
				activeLight->SetDiffusiveIntensity(diffI);

				float specCol[3] = { activeLight->GetSpecularColor().x, activeLight->GetSpecularColor().y, activeLight->GetSpecularColor().z };
				float specI = activeLight->GetSpecularIntensity();
				ImGui::Text("Active Specular Color");
				ImGui::ColorEdit3("ASC", specCol);
				ImGui::Text("Active Specular Intensity");
				ImGui::SliderFloat("ASI", &specI, 0, 2);
				activeLight->SetSpecularColor({ specCol[0], specCol[1], specCol[2], 1 });
				activeLight->SetSpecularIntensity(specI);
			}

			if (ImGui::Button("Next light model"))
			{
				scene->NextLight();
			}
			if (ImGui::Button("Delete light model"))
			{
				scene->DeleteActiveLight();
				if (scene->GetActiveLightIndex() == DISABLED)
				{
					lightControlFrame = false;
				}
			}
			ImGui::Text("Active light: %d", scene->GetActiveLightIndex());

			static glm::mat4x4 activeLightModelTransformation;
			activeLightModelTransformation = scene->GetActiveLightModelTransformation();
			std::string sLightModelTransformation = "";
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					sLightModelTransformation.append(std::to_string(activeLightModelTransformation[i][j]) + " ");
				}
				sLightModelTransformation.append("\n");
			}

			ImGui::Text("Light Model transformation:\n");
			ImGui::Text(sLightModelTransformation.c_str());

		}

		ImGui::End();
	}

	if (colorMenu)
	{
		ImGui::Begin("Color menu");

		ImGui::Text("-------------- Background: --------------");
		glm::vec4 currentBackgroundColor = scene->GetBackgroundColor();
		static float background[3] = { currentBackgroundColor.x ,currentBackgroundColor.y ,currentBackgroundColor.z };
		ImGui::ColorEdit3("BG", background);
		scene->SetBackgroundColor({ background[0], background[1], background[2], 1 });

		ImGui::Text("-------------- Polygons: --------------");
		glm::vec4 currentPolygonCol = scene->GetPolygonColor();
		static float polygon[3] = { currentPolygonCol.x ,currentPolygonCol.y ,currentPolygonCol.z };
		ImGui::ColorEdit3("Polygon", polygon);
		scene->SetPolygonColor({ polygon[0], polygon[1], polygon[2], 1 });

		ImGui::Text("-------------- Wireframe: --------------");
		glm::vec4 currentWireframeCol = scene->GetWireframeColor();
		static float wireframe[3] = { currentWireframeCol.x ,currentWireframeCol.y ,currentWireframeCol.z };
		ImGui::ColorEdit3("Wireframe", wireframe);
		scene->SetWireframeColor({ wireframe[0], wireframe[1], wireframe[2], 1 });

		ImGui::End();
	}

	// Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "CTRL+O"))
				{
					nfdchar_t *outPath = nullptr;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", nullptr, &outPath);
					if (result == NFD_OKAY) {
						ImGui::Text("Hello from another window!");
						scene->AddModel(std::make_shared<MeshModel>(Utils::LoadMeshModel(outPath, material)));
						modelControlWindow = true;
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::MenuItem("Colors"))
				{
					colorMenu = true;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Show Demo Menu")) { showDemoWindow = true; }
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}