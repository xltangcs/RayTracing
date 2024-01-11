#include "SceneObject.h"

Sphere::Sphere(glm::vec3 pos, float r, int materialIndex)
	:m_Position(pos), m_Radius(r), m_MaterialIndex(materialIndex)
{
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
		return true;
	}

	return false;
}
