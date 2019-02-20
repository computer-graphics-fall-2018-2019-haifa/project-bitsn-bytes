#pragma once

#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include <string>
#include <stdlib.h>
#include <map>

// Screen Sizes & Resolutions
#define DEFAULT_HEIGHT						720
#define DEFAULT_WIDTH						1280
#define MAX_HEIGHT_4K						2160
#define MAX_WIDTH_4K						3840
// Titles & Descriptions
#define WINDOW_TITLE						"Mesh Viewer"
// Source files
#define CAMERA_SOURCE						"..\\Data\\camera.obj"
#define SPHERE_SOURCE						"..\\Data\\sphere.obj"
#define CUBE_SOURCE							"..\\Data\\cube.obj"
// Constants
#define DISABLED							-1
#define PI									3.141592653589793238462643383279502884L
#define FACE_ELEMENTS						3
// Constant matrices
#define ZERO_MATRIX							{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }
#define FLATTEN_MATRIX						{ { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 1 } }
#define I_MATRIX							{ { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } }
// Functions
#define SCALING_MATRIX4(value)				{ { value, 0, 0, 0 }, { 0, value, 0, 0 }, { 0, 0, value, 0 }, { 0, 0, 0, 1 } }
#define TRANSLATION_MATRIX(x, y, z)			{ { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { (x), (y), (z), 1 } }
#define HOMOGENEOUS_VECTOR4					{ 0.0f, 0.0f, 0.0f, 1.0f }
#define HOMOGENEOUS_MATRIX4(x, y, z, w)		{ { (x), 0, 0, 0 }, { 0, (y), 0, 0 }, { 0, 0, (z), 0 }, { 0, 0, 0, (w) } }
#define ROTATING_MATRIX_X_AXIS(angle)		{ { 1, 0, 0, 0}, { 0, cos(angle), sin(angle), 0 }, { 0, -sin(angle), cos(angle), 0 }, { 0, 0, 0, 1 } }
#define ROTATING_MATRIX_Y_AXIS(angle)		{ { cos(angle), 0, -sin(angle), 0 }, { 0, 1, 0, 0 }, { sin(angle), 0, cos(angle), 0 }, { 0, 0, 0, 1 } }
#define ROTATING_MATRIX_Z_AXIS(angle)		{ { cos(angle), sin(angle), 0, 0 }, { -sin(angle), cos(angle), 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } }
#define PERSPECTIVE_MATRIX(d)				{ { 1, 0, 0, 0}, { 0, 1, 0, 0 }, { 0, 0, 1, 1.0f / (d) }, { 0, 0, 0, 0 } }
#define NORM_ZERO_TO_ONE(value, min, max)	((value) - (min)) / ((max) - (min))
#define NORMALIZE_COORDS(value, min, max)	(NORM_ZERO_TO_ONE(value, min, max) * 2) - 1
#define MIN(a, b)							a < b ? a : b
#define MAX(a, b)							a > b ? a : b
#define TO_RADIAN(angle)					angle * PI / 180.0f
#define INDEX(width, x, y, c)				(x + y * width) * 3 + c
#define COLOR(color)						Utils::GetColor(color)

// Enumerators
typedef enum _COLOR_ {

	WHITE,
	RED,
	BLACK,
	GREEN,
	YELLOW,
	BLUE,
	LIME,
	X_COL,
	Y_COL,
	Z_COL

} COLOR;

typedef enum _RETURN_VALUE_ {

	SUCCESS = 0,
	FAILURE,
	IO_ERROR,
	UNDEFINED

} RETURN_VALUE;

typedef enum _PRIMITIVE_ {

	SPHERE = 0,
	CUBE,
	CAMERA

} PRIMITIVE;

typedef struct _PROJECTION_PARAMETERS_
{
	float left;
	float right;
	float bottom;
	float top;
	float zNear;
	float zFar;

} PROJECTION_PARAMETERS, *PPROJECTION_PARAMETERS;

typedef struct _PERSPECTIVE_PARAMETERS_
{
	float fovy;
	float aspect;
	float zNear;
	float zFar;

} PERSPECTIVE_PARAMETERS, *PPERSPECTIVE_PARAMETERS;

typedef struct _CUBE_LINES_
{
	std::pair<glm::vec3, glm::vec3> line[12];

} CUBE_LINES, *PCUBE_LINES;

typedef enum _AXIS_
{
	X,
	Y,
	Z,
	XY,
	YZ,
	ZX
} AXIS;

typedef enum _ROTATION_REL_
{
	WORLD,
	SELF
} ROTATION_REL;

typedef enum _FRAME_TYPE_
{
	CAMERA,
	MODEL,
	WORLD,
	LIGHT
} FRAME_TYPE;

typedef enum _SHADING_TYPE_
{
	NONE,
	SOLID,
	FLAT,
	PHONG,
	GOURAUD
} SHADING_TYPE;

typedef enum _LIGHT_TYPE_
{
	AMBIENT,
	SPECULAR,
	DIFFUSIVE
} LIGHT_TYPE;

typedef enum _LIGHT_SOURCE_TYPE_
{
	POINT,
	PARALLEL,
	AREA
} LIGHT_SOURCE_TYPE;

typedef enum _GENERATED_TEXTURE_
{
	NONE,
	CRYSTAL,
	RUG
} GENERATED_TEXTURE;

typedef struct _LIGHT_DATA_
{
	float     intensity;
	glm::vec4 color;


} LIGHT_DATA, *PLIGHT_DATA;

typedef struct _LIGHT_INFO_
{
	LIGHT_DATA diffusive;
	LIGHT_DATA specular;
	LIGHT_DATA ambient;

	glm::vec3 location;
	LIGHT_SOURCE_TYPE lightSourceType;

} LIGHT_INFO, *PLIGHTS_INFO;

const std::map<PRIMITIVE, std::string> PRIMITIVES = {

	{SPHERE, SPHERE_SOURCE},
	{CUBE, CUBE_SOURCE},
	{CAMERA, CAMERA_SOURCE}

};

typedef enum _POST_EFFECT_
{
	NONE = 0,
	BLUR_SCENE,
	BLOOM
} POST_EFFECT, *PPOST_EFFECT;

#define SET_PROJECTION_PARAMETERS(parameters)                                \
                                         float left     = parameters.left;   \
                                         float right    = parameters.right;  \
                                         float top      = parameters.top;    \
                                         float bottom   = parameters.bottom; \
                                         float zNear    = parameters.zNear;  \
                                         float zFar     = parameters.zFar;   \

#define INIT_CUBE_COORDINATES(maxCoordinates, minCoordinates)                \
                                         float cRight   = maxCoordinates.x;  \
                                         float cTop     = maxCoordinates.y;  \
                                         float cFar     = maxCoordinates.z;  \
                                         float cLeft    = minCoordinates.x;  \
                                         float cBottom  = minCoordinates.y;  \
                                         float cNear	= minCoordinates.z;  \

#define SET_PERSPESCTIVE_PARAMETERS(parameters)                              \
                                         float fovy		= parameters.fovy;   \
                                         float aspect   = parameters.aspect; \
                                         float zNear    = parameters.zNear;  \
                                         float zFar     = parameters.zFar;   \

#endif // !__CONSTANTS_H__