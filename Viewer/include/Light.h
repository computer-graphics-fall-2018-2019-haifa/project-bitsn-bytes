#pragma once

#include <glm/glm.hpp>

class Light {

	public:
		Light();
		Light(const glm::vec3& color);
		virtual ~Light();

		const glm::vec3& GetColor()  const;
		void SetColor(const glm::vec3& color);

	protected:
		glm::vec3 color;

};

class PointLight : public Light {

	public:
		PointLight(const glm::vec3& position);
		PointLight(const glm::vec3& position, const glm::vec3& color);
		virtual ~PointLight();
		glm::vec3& GetPosition();

	private:
		glm::vec3 position;

};

class AmbientLight : public Light {

	public:
		AmbientLight();
		virtual ~AmbientLight();

};