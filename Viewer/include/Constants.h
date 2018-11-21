#pragma once

#include <string>
#include <stdlib.h>

// Screen Sizes & Resolutions
#define DEFAULT_HEIGHT						720
#define DEFAULT_WIDTH						1280
#define MAX_HEIGHT_4K						2160
#define MAX_WIDTH_4K						3840
// Titles & Descriptions
#define WINDOW_TITLE						"Mesh Viewer"
// Source files
#define CAMERA_SOURCE						"Data/camera.obj"
// Constants
#define DISABLED							-1
#define PI									3.141592653589793238462643383279502884L
// Constant matrices
#define ZERO_MATRIX							{ { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }
#define FLATTEN_MATRIX						{ { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 1 } }
#define I_MATRIX							{ { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } }
// Functions
#define SCALING_MATRIX4(value)				{ { (value), 0, 0, 0 }, { 0, (value), 0, 0 }, { 0, 0, (value), 0 }, { 0, 0, 0, 1 } }
#define TRANSLATION_MATRIX(x, y, z)			{ { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { (x), (y), (z), 1 } }
#define HOMOGENEOUS_VECTOR4					{ 0.0f, 0.0f, 0.0f, 1.0f }
#define HOMOGENEOUS_MATRIX4(x, y, z, w)		{ { (x), 0, 0, 0 }, { 0, (y), 0, 0 }, { 0, 0, (z), 0 }, { 0, 0, 0, (w) } }
#define ROTATING_MATRIX_X_AXIS(angle)		{ { 1, 0, 0, 0}, { 0, cos(angle), sin(angle), 0 }, { 0, -sin(angle), cos(angle), 0 }, { 0, 0, 0, 1 } }
#define ROTATING_MATRIX_Y_AXIS(angle)		{ { cos(angle), 0, -sin(angle), 0 }, { 0, 1, 0, 0 }, { sin(angle), 0, cos(angle), 0 }, { 0, 0, 0, 1 } }
#define ROTATING_MATRIX_Z_AXIS(angle)		{ { cos(angle), sin(angle), 0, 0}, { -sin(angle), cos(angle), 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } }
#define PERSPECTIVE_MATRIX(d)				{ { 1, 0, 0, 0}, { 0, 1, 0, 0 }, { 0, 0, 1, 1.0f / (d) }, { 0, 0, 0, 0 } }
#define NORM_ZERO_TO_ONE(value, min, max)	((value) - (min)) / ((max) - (min))
#define NORMALIZE_COORDS(value, min, max)	(((NORM_ZERO_TO_ONE(value, min, max) * 2) - 1))
#define MIN(a, b)							(((a) < (b)) ? (a) : (b))
#define MAX(a, b)							(((a) > (b)) ? (a) : (b))

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

typedef enum _PRIM_MODEL_ {

	SPHERE = 0,
	CUBE,
	CAMERA

} PRIM_MODEL;

typedef struct _PROJ_PARAMS_
{
	float left;
	float right;
	float bottom;
	float top;
	float zNear;
	float zFar;

} PROJ_PARAMS, *PPROJ_PARAMS;