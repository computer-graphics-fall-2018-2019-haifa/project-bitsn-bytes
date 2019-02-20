#pragma once

#ifndef __FACE_H__
#define __FACE_H__

#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Utils.h";

class Surface
{
public:
	std::string m_material;
	glm::vec4 m_ambientColor;
	glm::vec4 m_diffuseColor;
	glm::vec4 m_specularColor;

	float m_ambientReflectionRate;
	float m_diffuseReflectionRate;
	float m_specularReflectionRate;
	int m_shininess;

	Surface() : m_material("Empty"),
		m_ambientReflectionRate(1.f),
		m_ambientColor(Utils::GetColor(COLOR::BLACK)),
		m_diffuseReflectionRate(1.f),
		m_diffuseColor(Utils::GetColor(COLOR::BLACK)),
		m_specularReflectionRate(0.2f),
		m_specularColor(Utils::GetColor(COLOR::BLACK)),
		m_shininess(1)
	{}

	Surface(const std::string& material,
		const glm::vec4& ambientC, float ambientI,
		const glm::vec4& diffusiveC, float diffusiveI,
		const glm::vec4& specularC, float specularI,
		int shininess) :
		m_material(material),
		m_ambientColor(ambientC), m_ambientReflectionRate(ambientI),
		m_diffuseColor(diffusiveC), m_diffuseReflectionRate(diffusiveI),
		m_specularColor(specularC), m_specularReflectionRate(specularI),
		m_shininess(shininess)
	{}

	Surface(const Surface& surf) :
		Surface(surf.m_material,
			surf.m_ambientColor, surf.m_ambientReflectionRate,
			surf.m_diffuseColor, surf.m_diffuseReflectionRate,
			surf.m_specularColor, surf.m_specularReflectionRate,
			surf.m_shininess)
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
};

#endif // !__FACE_H__