#pragma once

#ifndef __FACE_H__
#define __FACE_H__

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Constants.h"
#include "Utils.h"

class Surface
{
public:
	std::string material;
	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;

	float ambientReflectionRate;
	float diffuseReflectionRate;
	float specularReflectionRate;
	int shininess;

	Surface() : material("Empty"),
		ambientReflectionRate(1.f),
		ambientColor(Utils::GetColor(COLOR::BLACK)),
		diffuseReflectionRate(1.f),
		diffuseColor(Utils::GetColor(COLOR::BLACK)),
		specularReflectionRate(0.2f),
		specularColor(Utils::GetColor(COLOR::BLACK)),
		shininess(1)
	{}

	Surface(const std::string& material,
		const glm::vec4& ambientC, float ambientI,
		const glm::vec4& diffusiveC, float diffusiveI,
		const glm::vec4& specularC, float specularI,
		int shininess) :
		material(material),
		ambientColor(ambientC), ambientReflectionRate(ambientI),
		diffuseColor(diffusiveC), diffuseReflectionRate(diffusiveI),
		specularColor(specularC), specularReflectionRate(specularI),
		shininess(shininess)
	{}

	Surface(const Surface& surf) :
		Surface(surf.material,
			surf.ambientColor, surf.ambientReflectionRate,
			surf.diffuseColor, surf.diffuseReflectionRate,
			surf.specularColor, surf.specularReflectionRate,
			surf.shininess)
	{}

	~Surface() = default;

};

class Face
{
private:
	std::vector<int> vertexIndices;
	std::vector<int> normalIndices;
	std::vector<int> textureIndices;

public:
	Face(std::istream& issLine);
	virtual ~Face();
	const int Face::GetVertexIndex(int index);
	const int Face::GetNormalIndex(int index);
	const int Face::GetTextureIndex(int index);

	glm::vec4 actualColorP1;
	glm::vec4 actualColorP2;
	glm::vec4 actualColorP3;
	Surface* surface;
	std::vector <std::pair<std::pair<float, glm::vec4>, std::pair<glm::vec3, glm::mat4x4>>> diffusiveColorAndSource;
	std::vector <std::pair<std::pair<float, glm::vec4>, std::pair<glm::vec3, glm::mat4x4>>> speculativeColorAndSource;
};

#endif // !__FACE_H__