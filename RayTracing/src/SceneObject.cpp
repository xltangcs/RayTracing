#include "SceneObject.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtx/intersect.hpp>

//bool intersectRayTriangle(glm::vec3 orig, glm::vec3 orig, glm::vec3 orig, glm::vec3 orig, glm::vec3 orig, )

AABB::AABB(const glm::vec3& minbound, const glm::vec3& maxbound)
	:MaxBound(maxbound), MinBound(minbound)
{
}

bool AABB::Hit(const Ray& ray, float tmin, float tmax) const
{
	for (int i = 0; i < 3; i++)
	{
		float invD = 1.0f / ray.Direction[i];
		float t0 = (MinBound[i] - ray.Origin[i]) * invD;
		float t1 = (MaxBound[i] - ray.Origin[i]) * invD;

		if (invD < 0.0f)
			std::swap(t0, t1);
		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;
		if (tmax <= tmin) return false;
	}
	return true;
}

void AABB::Padding()
{
	float delta = 0.001f;
	for (int i = 0; i < 3; i++)
	{
		if (std::fabs(MaxBound[i] - MinBound[i]) < delta)
		{
			MaxBound[i] += delta / 2.0f;
			MinBound[i] -= delta / 2.0f;
		}
	}
}


Sphere::Sphere()
{
	m_Type = "Sphere";
	m_MaterialIndex = 0;
}

Sphere::Sphere(glm::vec3 pos, float r, int materialIndex)
	:m_Position(pos), m_Radius(r)
{
	m_Type = "Sphere";
	m_MaterialIndex = materialIndex;
}

bool Sphere::Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const
{
	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2 * glm::dot(ray.Direction, ray.Origin - m_Position);
	float c = glm::dot(ray.Origin - m_Position, ray.Origin - m_Position) - m_Radius * m_Radius;

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant < 0) return false;

	float t0 = (-b - glm::sqrt(discriminant)) / (2.0f * a); 

	if ((t0 < t_max) && t0 > t_min)
	{
		payload.HitDistance = t0;
		payload.WorldPosition = ray.Origin + ray.Direction * t0;

		glm::vec3 normal = (payload.WorldPosition - m_Position) / m_Radius;
		payload.FrontFace = glm::dot(ray.Direction, normal) < 0;
		payload.WorldNormal = payload.FrontFace ? normal : -normal;
		payload.MaterialIndex = m_MaterialIndex;
		GetUV(normal, payload.UV);

		//glm::vec3 origin = ray.Origin - m_Position;
		//payload.WorldPosition = origin + ray.Direction * t0;
		//payload.WorldNormal = glm::normalize(payload.WorldPosition); //normal
		//payload.MaterialIndex = m_MaterialIndex;

		return true;
	}
	float t1 = (-b + glm::sqrt(discriminant)) / (2.0f * a);

	if ((t1 < t_max) && t1 > t_min)
	{
		payload.HitDistance = t1;
		payload.WorldPosition = ray.Origin + ray.Direction * t1;

		glm::vec3 normal = (payload.WorldPosition - m_Position) / m_Radius;
		payload.FrontFace = glm::dot(ray.Direction, normal) < 0;
		payload.WorldNormal = payload.FrontFace ? normal : -normal;
		payload.MaterialIndex = m_MaterialIndex;
		GetUV(normal, payload.UV);

		return true;
	}

	return false;
}

bool Sphere::BoundingBox(float t0, float t1, AABB& output_box) const
{
	output_box = AABB( m_Position - glm::vec3(m_Radius), m_Position + glm::vec3(m_Radius) );
	return true;
}

void Sphere::GetUV(const glm::vec3& pos, glm::vec2& uv)
{
	auto theta = acos(-pos.y);
	auto phi = atan2(-pos.z, pos.x) + glm::pi<float>();
	uv.x = phi / (2 * glm::pi<float>());
	uv.y = theta / glm::pi<float>();

	//auto phi = atan2(pos.z, pos.x);
	//auto theta = asin(pos.y);
	//uv.x = 1.0f - (phi + glm::pi<float>()) / (2.0f * glm::pi<float>());
	//uv.y = (theta + glm::pi<float>() / 2.0f) / glm::pi<float>();

}

Quad::Quad()
{
	m_Type = "Quad";
	m_MaterialIndex = 0;
}

Quad::Quad(glm::vec3 pos, glm::vec3 udir, glm::vec3 vdir, int materialIndex)
	:m_Position(pos), m_UDirection(udir), m_VDirection(vdir)
{
	m_Type = "Quad";
	m_MaterialIndex = materialIndex;
}

bool Quad::Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const
{
	glm::vec3 p1 = m_Position + m_UDirection;
	glm::vec3 p2 = m_Position + m_VDirection;
	glm::vec3 p3 = m_Position + m_UDirection + m_VDirection;
	glm::vec2 baryPosition;

	float t;
	bool isHit = glm::intersectRayTriangle(ray.Origin, ray.Direction, m_Position, p1, p3, baryPosition, t);
	if (!isHit)
	{
		isHit = glm::intersectRayTriangle(ray.Origin, ray.Direction, m_Position, p2, p3, baryPosition, t);
		if (!isHit)
		{
			return false;
		}
	}

	if (t < t_min || t > t_max) return false;

	payload.HitDistance = t;
	payload.WorldPosition = ray.Origin + t * ray.Direction;

	glm::vec3 edge1 = m_Position - payload.WorldPosition;
	glm::vec3 edge2 = p1 - payload.WorldPosition;
	glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

	payload.FrontFace = glm::dot(normal, ray.Direction) < 0; 
	payload.WorldNormal = payload.FrontFace ? normal : -normal;
	payload.MaterialIndex = m_MaterialIndex;
	payload.UV = baryPosition;

	return true;
/*
	auto normal = glm::cross(m_UDirection, m_VDirection);
	auto d = glm::dot(normal, m_Position);

	auto w = normal / glm::dot(normal, normal);
	auto denom = glm::dot(normal, ray.Direction);

	if (fabs(denom) < 1e-8)
	{
		return false;
	}

	auto t = (d - glm::dot(normal, ray.Origin)) / denom;
	if (t < t_min || t > t_max) return false;

	glm::vec3 hitpos = ray.Origin + t * ray.Direction;
	glm::vec3 plannarvector = hitpos - m_Position;
	float alpha = glm::dot(w, glm::cross(plannarvector, m_VDirection));
	float beta  = glm::dot(w, glm::cross(m_UDirection, plannarvector));

	if (!GetUV(alpha, beta, payload.UV)) return false;

	payload.HitDistance = t;
	payload.WorldPosition = ray.Origin + t * ray.Direction;
	payload.MaterialIndex = m_MaterialIndex;
	payload.FrontFace = glm::dot(ray.Direction, normal) < 0;
	payload.WorldNormal = payload.FrontFace ? normal : -normal;

	return true;
	*/
}

bool Quad::BoundingBox(float t0, float t1, AABB& output_box) const
{
	output_box = AABB(m_Position, m_Position + m_UDirection + m_VDirection);
	output_box.Padding();
	return true;
}

bool Quad::GetUV(float a, float b, glm::vec2& uv)
{
	if ((a < 0.0f) || (1.0f < a) || (b < 0.0f) || (1.0f < b))
		return false;

	uv.x = a;
	uv.y = b;
	return true;
}
