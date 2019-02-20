#pragma once

#ifndef __UTILS_H__
#define __UTILS_H__

#include <glm/glm.hpp>
#include <string>
#include "MeshModel.h"

/*
 * Utils class.
 * This class is consisted of static helper methods that can have many clients across the code.
 */
class Utils
{
	public:
		static glm::vec3 Vec3fFromStream(std::istream& issLine);
		static glm::vec2 Vec2fFromStream(std::istream& issLine);
		static MeshModel LoadMeshModel(const std::string& filePath, const Surface& material, GLuint program);

		static glm::vec4 ToHomogeneousForm(const glm::vec3& normalForm);
		static glm::vec4 ExpandToVec4(const glm::vec3& vector);
		static glm::vec3 ToCartesianForm(const glm::vec4& homogeneousForm);
		static glm::vec4 Cross(const glm::vec4 & lVector4, const glm::vec4 & rVector4);
		static glm::vec3 FindCentralVec(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

		static bool IsVecEqual(glm::vec3 v1, glm::vec3 v2);
		static bool IsVecEqual(glm::vec4 v1, glm::vec4 v2);
		
		static glm::vec4 GetColor(COLOR color);

	private:
		static std::string GetFileName(const std::string& filePath);
		static std::string GetWorkingDirectory();
};

#endif // !__UTILS_H__
