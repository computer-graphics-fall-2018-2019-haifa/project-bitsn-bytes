#include "Utils.h"
#include "Constants.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

MeshModel Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());

	if (ifile.fail()) {
		fprintf(stderr, "An error occured while trying to open %s", Utils::GetFileName(filePath));
		exit(-1);
	}

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			glm::vec3 parsedVector = Vec3fFromStream(issLine);
			vertices.push_back(parsedVector);
		}
		else if (lineType == "vn")
		{
			normals.push_back(Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// Texture coordinates
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	return MeshModel(faces, vertices, normals, Utils::GetFileName(filePath));
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}

glm::vec4 Utils::ToHomogeneousForm(const glm::vec3& normalForm)
{
	return glm::vec4(normalForm.x, normalForm.y, normalForm.z, 1);
}

glm::vec4 Utils::ExpandToVec4(const glm::vec3 & vector)
{
	return glm::vec4(vector.x, vector.y, vector.z, 0);
}

glm::vec3 Utils::ToCartesianForm(const glm::vec4& homogeneousForm)
{
	return glm::vec3(homogeneousForm.x / homogeneousForm[3], homogeneousForm.y / homogeneousForm[3], homogeneousForm.z / homogeneousForm[3]);
}

glm::vec4 Utils::Cross(const glm::vec4 & lVector4, const glm::vec4 & rVector4)
{
	return ToHomogeneousForm(glm::cross(ToCartesianForm(lVector4), ToCartesianForm(rVector4)));
}

glm::vec3 Utils::FindCentralVec(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	return 0.5f*(p2 + p3) - p1;
}

bool Utils::IsVecEqual(glm::vec3 v1, glm::vec3 v2)
{
	bool bEquals = true;

	bEquals &= fabs(v1.x - v2.x) < std::numeric_limits<float>::epsilon();
	bEquals &= fabs(v1.y - v2.y) < std::numeric_limits<float>::epsilon();
	bEquals &= fabs(v1.z - v2.z) < std::numeric_limits<float>::epsilon();

	return bEquals;


}

bool Utils::IsVecEqual(glm::vec4 v1, glm::vec4 v2)
{
	bool bEquals = true;

	bEquals &= fabs(v1.x - v2.x) < std::numeric_limits<float>::epsilon();
	bEquals &= fabs(v1.y - v2.y) < std::numeric_limits<float>::epsilon();
	bEquals &= fabs(v1.z - v2.z) < std::numeric_limits<float>::epsilon();
	bEquals &= fabs(v1.w - v2.w) < std::numeric_limits<float>::epsilon();

	return bEquals;
}

glm::vec3 Utils::GetColor(COLOR color)
{
	switch (color)
	{
		case WHITE:
			return { 1.0f, 1.0f, 1.0f };
		case BLACK:
			return { 0.f, 0.f, 0.f };
		case LIME:
			return { 0, 1.0f ,0 };
		case GREEN:
			return { 0, 0.5f, 0 };
		case BLUE:
			return { 0, 0 ,1.0f };
		case RED:
			return { 1.0f, 0 ,0 };
		case YELLOW:
			return { 1.0f,1.0f,0 };
		case X_COL:
			return { 1.f,102.f / 255.f,0.f };
		case Y_COL:
			return { 153.f / 255.f,204.f / 255.f,0.f };
		case Z_COL:
			return { 51.f / 255.f, 102.f / 255, 1.f };


		default:
			return { 0.f, 0.f, 0.f };

	}
}