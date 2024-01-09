#include "Renderer.h"
#include <execution>

namespace Utils {

	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;
		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Toffee::Image>(width, height, Toffee::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
	m_ActiveCamera = &camera;
	m_ActiveScene = &scene;

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			//for(int sample)
			glm::vec4 color = PerPixel(x, y);

			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y)
{
	Ray ray;
	ray.Origin = m_ActiveCamera->GetPosition();
	ray.Direction = m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];

	glm::vec3 redColor(1.0f,0.0f, 0.0f);
	glm::vec3 skyColor(0.6f, 0.7f, 0.9f);

	Renderer::HitPayload payload = TraceRay(ray);
	if (payload.HitDistance < -0.0f)
	{
		return glm::vec4(skyColor, 1.0f);
	}
	const Sphere& sphere = m_ActiveScene->Spheres[payload.ObjectIndex];
	const Material& material = m_ActiveScene->Materials[sphere.MaterialIndex];

	return glm::vec4(material.Albedo, 1.0f);
}

Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
{
	int closestSphere = -1;
	float hitDistance = std::numeric_limits<float>::max();

	for(size_t i = 0; i<m_ActiveScene->Spheres.size(); i++)
	{
		Sphere sphere = m_ActiveScene->Spheres[i];
		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2 * glm::dot(ray.Direction, ray.Origin - sphere.Position);
		float c = glm::dot(ray.Origin - sphere.Position, ray.Origin - sphere.Position) - sphere.Radius * sphere.Radius;

		float discriminant = b * b - 4.0f * a * c;

		if (discriminant < 0) continue;

		float t0 = (-b - glm::sqrt(discriminant)) / (2.0f * a); //closer

		if (t0 > 0.0f && t0 < hitDistance)
		{
			hitDistance = t0;
			closestSphere = (int)i;
		}
	}
	if (closestSphere < 0)
		return Miss(ray);
	return ClosestHit(ray, hitDistance, closestSphere);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
	Renderer::HitPayload payload;
	payload.HitDistance = hitDistance;
	payload.ObjectIndex = objectIndex;

	const Sphere& closestSphere = m_ActiveScene->Spheres[objectIndex];

	glm::vec3 origin = ray.Origin - closestSphere.Position;
	payload.WorldPosition = origin + ray.Direction * hitDistance;  
	payload.WorldNormal = glm::normalize(payload.WorldPosition); //normal

	payload.WorldPosition += closestSphere.Position;
	return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray& ray)
{
	Renderer::HitPayload payload;
	payload.HitDistance = -1.0f;
	return payload;
}

