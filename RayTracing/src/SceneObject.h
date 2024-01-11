#pragma once

#include "glm/glm.hpp"

#include "Ray.h"
#include "HitPayload.h"
#include "Material.h"


class SceneObject {
public:
	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const = 0;
};

class Sphere : public SceneObject {
public:
	Sphere() = default;
	Sphere(glm::vec3 pos, float r, int materialIndex);

	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const;

	glm::vec3& GetPosition() { return m_Position; }
	float& GetRadius() { return m_Radius; }
	int& GetMaterialIndex() { return m_MaterialIndex; }

private:
    glm::vec3 m_Position;
    float m_Radius;
	int m_MaterialIndex = 0;
};
