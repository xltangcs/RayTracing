#pragma once

#include <string>

#include "glm/glm.hpp"

#include "Ray.h"
#include "HitPayload.h"

class AABB
{
public:
	AABB() = default;
	AABB(const glm::vec3& minbound, const glm::vec3& maxbound);

	bool Hit(const Ray& ray, float tmin, float tmax) const;
	void Padding();

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

public:
	std::string m_Type;
	int m_MaterialIndex;
};

class Sphere : public SceneObject {
public:
	Sphere();
	Sphere(glm::vec3 pos, float r, int materialIndex);

	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const;
	virtual bool BoundingBox(float t0, float t1, AABB& output_box) const;

	glm::vec3& GetPosition() { return m_Position; }
	float& GetRadius() { return m_Radius; }
private:
	static void GetUV(const glm::vec3& pos, glm::vec2& uv);
private:
    glm::vec3 m_Position;
    float m_Radius;
};

class Quad : public SceneObject {
public:
	Quad();
	Quad(glm::vec3 pos, glm::vec3 udir, glm::vec3 vdir, int materialIndex);

	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const;
	virtual bool BoundingBox(float t0, float t1, AABB& output_box) const;

	glm::vec3& GetPosition() { return m_Position; }
	glm::vec3& GetUDirection() { return m_UDirection; }
	glm::vec3& GetVDirection() { return m_VDirection; }

private:
	static bool GetUV(float a, float b, glm::vec2& uv);
private:
	glm::vec3 m_Position;
	glm::vec3 m_UDirection, m_VDirection;
};

