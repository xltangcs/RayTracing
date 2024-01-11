#include "Material.h"

Lambertian::Lambertian(const glm::vec3& albedo)
	:m_Albedo(albedo)
{
}

bool Lambertian::Scatter(const Ray& rayIn, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const
{
	glm::vec3 scatter_direction = payload.WorldNormal + Toffee::Random::InUnitSphere();
	scattered = Ray(payload.WorldPosition, scatter_direction);
	attenuation = m_Albedo;
	return true;
}

Metal::Metal(const glm::vec3& albedo, const float& roughness)
	:m_Albedo(albedo), m_Roughness(roughness)
{
}

bool Metal::Scatter(const Ray& rayIn, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const
{
	glm::vec3 reflected = glm::reflect(glm::normalize(rayIn.Direction), payload.WorldNormal);
	scattered = Ray(payload.WorldPosition, reflected + m_Roughness * Toffee::Random::InUnitSphere());

	//glm::vec3 reflected = glm::reflect(glm::normalize(rayIn.Direction), payload.WorldNormal + +m_Roughness * Toffee::Random::InUnitSphere());
	//scattered = Ray(payload.WorldPosition, reflected);

	attenuation = m_Albedo;
	return (glm::dot(scattered.Direction, payload.WorldNormal) > 0);
}

Dielectric::Dielectric(double refractivity)
	:m_Refractivity(refractivity)
{
}

bool Dielectric::Scatter(const Ray& rayIn, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const
{
	attenuation = glm::vec3(1.0f);
	double etai_over_etat = payload.FrontFace ? 1.0 / m_Refractivity : m_Refractivity;

	double cos_theta = std::fmin(glm::dot(glm::normalize(rayIn.Direction), payload.WorldNormal), 1.0f);
	double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	if (etai_over_etat * sin_theta > 1.0)
	{
		glm::vec3 reflected = glm::reflect(glm::normalize(rayIn.Direction), payload.WorldNormal);
		scattered = Ray(payload.WorldPosition, reflected);
		return true;
	}

	double reflect_prob = schlick(cos_theta, etai_over_etat); //There is a probability that glass in reality will reflect

	if (Toffee::Random::Double() < reflect_prob)
	{
		glm::vec3 reflected = glm::reflect(glm::normalize(rayIn.Direction), payload.WorldNormal);
		scattered = Ray(payload.WorldPosition, reflected);
		return true;
	}

	glm::vec3 refracted = glm::refract(glm::normalize(rayIn.Direction), payload.WorldNormal, (float)etai_over_etat);
	scattered = Ray(payload.WorldPosition, refracted);
	return true;
}

double Dielectric::schlick(double cosine, double ref_idx) const
{
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}