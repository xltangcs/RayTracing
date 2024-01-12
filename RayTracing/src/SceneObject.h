#pragma once

#include "glm/glm.hpp"

#include "Ray.h"
#include "HitPayload.h"

class AABB
{
public:
	AABB() = default;
	AABB(const glm::vec3& minbound, const glm::vec3& maxbound);

	bool Hit(const Ray& ray, float tmin, float tmax) const;

	glm::vec3 GetMaxBound() const { return MaxBound; }
	glm::vec3 GetMinBound() const { return MinBound; }

private:
	glm::vec3 MaxBound;
	glm::vec3 MinBound;
};

class SceneObject {
public:
	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const = 0;
	virtual bool BoundingBox(float t0, float t1, AABB& output_box) const = 0;
};

class Sphere : public SceneObject {
public:
	Sphere() = default;
	Sphere(glm::vec3 pos, float r, int materialIndex);

	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const;
	virtual bool BoundingBox(float t0, float t1, AABB& output_box) const;

	glm::vec3& GetPosition() { return m_Position; }
	float& GetRadius() { return m_Radius; }
	int& GetMaterialIndex() { return m_MaterialIndex; }

private:
    glm::vec3 m_Position;
    float m_Radius;
	int m_MaterialIndex = 0;
};

