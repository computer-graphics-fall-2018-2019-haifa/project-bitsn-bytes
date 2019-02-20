#include "Light.h"
#include <algorithm>

/* Light */

Light::Light() :
	color(glm::vec3(1.0, 1.0, 1.0))
{

}

Light::Light(const glm::vec3& color) :
	color(color)
{

}

Light::~Light() { }

const glm::vec3& Light::GetColor() const {
	return color;
}

void Light::SetColor(const glm::vec3& color) {
	this->color = color;
}

/* Point Light */

PointLight::PointLight(const glm::vec3& position) :
	Light(),
	position(position)
{

}

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color) :
	Light(color),
	position(position)
{

}

PointLight::~PointLight() { }

glm::vec3& PointLight::GetPosition() {
	return position;
}

/* Ambient Light */

AmbientLight::AmbientLight() :
	Light(glm::vec3(0.2, 0.2, 0.2))
{

}


AmbientLight::~AmbientLight() { }