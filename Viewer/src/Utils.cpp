#include "Utils.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

glm::vec3 Utils::Vec3fFromStream(istream& issLine)
{
	float x, y, z;
	issLine >> x >> ws >> y >> ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(istream& issLine)
{
	float x, y;
	issLine >> x >> ws >> y;
	return glm::vec2(x, y);
}

MeshModel Utils::LoadMeshModel(const string& filePath)
{
	vector<Face> faces;
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	ifstream ifile(filePath.c_str());

	if (ifile.fail()) {
		fprintf(stderr, "An error occured while trying to open %s", Utils::GetFileName(filePath));
		exit(-1);
	}

	glm::vec3 maxCoordinates = { -numeric_limits<float>::max(), -numeric_limits<float>::max(), -numeric_limits<float>::max() };
	glm::vec3 minCoordinates = { numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max() };

	glm::vec3 normalizedVector;
	glm::vec3 modelCentroid = { 0, 0, 0 };

	unsigned int verticesCount = 0;

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		string curLine;
		getline(ifile, curLine);

		// read the type of the line
		istringstream issLine(curLine);
		string lineType;

		issLine >> ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			glm::vec3 parsedVector = Vec3fFromStream(issLine);

			minCoordinates.x = _CMATH_::fmin(minCoordinates.x, parsedVector.x);
			minCoordinates.y = _CMATH_::fmin(minCoordinates.y, parsedVector.y);
			minCoordinates.z = _CMATH_::fmin(minCoordinates.z, parsedVector.z);

			maxCoordinates.x = _CMATH_::fmax(maxCoordinates.x, parsedVector.x);
			maxCoordinates.y = _CMATH_::fmax(maxCoordinates.y, parsedVector.y);
			maxCoordinates.z = _CMATH_::fmax(maxCoordinates.z, parsedVector.z);

			modelCentroid += parsedVector;

			verticesCount++;

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
			cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	MeshModel model = MeshModel(faces, vertices, normals, Utils::GetFileName(filePath));

	
}

string Utils::GetFileName(const string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == string::npos) {
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

		if (index == string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}