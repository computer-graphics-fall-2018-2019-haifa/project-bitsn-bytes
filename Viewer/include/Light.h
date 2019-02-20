#pragma once

#include <glm/glm.hpp>

#include "MeshModel.h"
#include "Constants.h"

class Light
{
	protected:

		LIGHT_SOURCE_TYPE type;

		LightModel* lightModel;

		glm::vec4 ambientLightColor;
		float ambientLightIntensity;

		glm::vec4 diffusiveLightColor;
		float diffusiveLightIntensity;

		glm::vec4 specularLightColor;
		float specularLightIntensity;

	public:

		Light(LIGHT_SOURCE_TYPE type, const glm::vec3& location,
			const glm::vec4& ambientC, float ambientI,
			const glm::vec4& diffusiveC, float diffusiveI,
			const glm::vec4& specularC, float specularI, GLuint prog) :
			lightModel(new LightModel(type, location, prog)),
			ambientLightColor(ambientC), ambientLightIntensity(ambientI),
			diffusiveLightColor(diffusiveC), diffusiveLightIntensity(diffusiveI),
			specularLightColor(specularC), specularLightIntensity(specularI) {}

		virtual ~Light() { delete lightModel; }

		LightModel&  GetLightModel() { return *lightModel; }

		void SetAmbientIntensity(float   intensity) { ambientLightIntensity = intensity; }
		void SetDiffusiveIntensity(float intensity) { diffusiveLightIntensity = intensity; }
		void SetSpecularIntensity(float  intensity) { specularLightIntensity = intensity; }

		float GetAmbientIntensity() { return ambientLightIntensity; }
		float GetDiffusiveIntensity() { return diffusiveLightIntensity; }
		float GetSpecularIntensity() { return specularLightIntensity; }

		void SetAmbientColor(const glm::vec4& color) { ambientLightColor = color; }
		void SetDiffusiveColor(const glm::vec4& color) { diffusiveLightColor = color; }
		void SetSpecularColor(const glm::vec4& color) { specularLightColor = color; }

		glm::vec4 GetAmbientColor() { return ambientLightColor; }
		glm::vec4 GetDiffusiveColor() { return diffusiveLightColor; }
		glm::vec4 GetSpecularColor() { return specularLightColor; }

		virtual void Illuminate(Face& polygon, const glm::mat4x4& lightModelTransf)
		{
			float ambientI = polygon.surface->ambientReflectionRate * ambientLightIntensity;
			glm::vec4 ambientC = polygon.surface->ambientColor + ambientLightColor;
			ambientC *= ambientI;

			polygon.actualColorP1 += ambientC;
			polygon.actualColorP2 += ambientC;
			polygon.actualColorP3 += ambientC;



			float diffusiveI = diffusiveLightIntensity * polygon.surface->diffuseReflectionRate;
			glm::vec4 diffusiveC = polygon.surface->diffuseColor + diffusiveLightColor;

			float speculativeI = specularLightIntensity * polygon.surface->specularReflectionRate;
			glm::vec4 speculativeC = polygon.surface->specularColor + specularLightColor;

			glm::vec3 centroid = lightModel->GetCentroid();
			polygon.diffusiveColorAndSource.push_back({ { diffusiveI, diffusiveC }, { centroid , lightModelTransf } });
			polygon.speculativeColorAndSource.push_back({ {speculativeI, speculativeC}, { centroid, lightModelTransf } });
		}
};

class PointSourceLight : public Light
{
	private:

		glm::vec3 lightSource = { 2.f, 2.f, 2.f };

	public:
		PointSourceLight(const glm::vec3& location,
			const glm::vec4& ambientC, float ambientI,
			const glm::vec4& diffusiveC, float diffusiveI,
			const glm::vec4& specularC, float specularI, GLuint prog) :
			Light(LIGHT_SOURCE_TYPE::POINT_, location, ambientC, ambientI, diffusiveC, diffusiveI, specularC, specularI, prog) {}
		~PointSourceLight() = default;
		void Illuminate(Face & polygon, const glm::mat4x4& lightModelTransf);
};

class ParallelSourceLight : public Light
{
	private:
		std::vector< glm::vec3 > lightSource;

	public:
		ParallelSourceLight(const glm::vec3& location,
			const glm::vec4& ambientC, float ambientI,
			const glm::vec4& diffusiveC, float diffusiveI,
			const glm::vec4& specularC, float specularI, GLuint prog) :
			Light(LIGHT_SOURCE_TYPE::PARALLEL, location, ambientC, ambientI, diffusiveC, diffusiveI, specularC, specularI, prog) {}
		~ParallelSourceLight() = default;
		void Illuminate(Face & polygon, const glm::mat4x4& lightModelTransf);
};

class DistributedSourceLight : public Light
{
	private:
		std::vector< glm::vec3 > lightSource;
	public:
		DistributedSourceLight(const glm::vec3& location,
			const glm::vec4& ambientC, float ambientI,
			const glm::vec4& diffusiveC, float diffusiveI,
			const glm::vec4& specularC, float specularI, GLuint prog) :
			Light(LIGHT_SOURCE_TYPE::AREA, location, ambientC, ambientI, diffusiveC, diffusiveI, specularC, specularI, prog) {}
		~DistributedSourceLight() = default;
		void Illuminate(Face & polygon, const glm::mat4x4& lightModelTransf);
};