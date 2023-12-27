#include "Renderer.h"

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
	Ray ray;
	ray.Origin = camera.GetPosition();
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

			glm::vec4 color = TraceRay(scene, ray);

			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::TraceRay(const Scene& scene, const Ray& ray)
{
	if(scene.Spheres.size() == 0)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	const Sphere* closestSphere = nullptr;
	float hitDistance = std::numeric_limits<float>::max();

	for (const Sphere& sphere : scene.Spheres)
	{
		float a = glm::dot(ray.Direction, ray.Direction);
		float b = 2 * glm::dot(ray.Direction, ray.Origin - sphere.Position);
		float c = glm::dot(ray.Origin - sphere.Position, ray.Origin - sphere.Position) - sphere.Radius * sphere.Radius;
		
		float discriminant = b * b - 4.0f * a * c;

		if (discriminant < 0) continue;

		float t0 = (-b - glm::sqrt(discriminant)) / (2.0f * a); //closer

		if (t0 < hitDistance)
		{
			hitDistance = t0;
			closestSphere = &sphere;
		}
 	}

	if(closestSphere == nullptr)
		return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	glm::vec3 origin = ray.Origin - closestSphere->Position;       
	glm::vec3 hitpoint = origin + ray.Direction * hitDistance;  //first hitpoint

	glm::vec3 normal = glm::normalize(hitpoint); //normal

	float intensity = glm::max(glm::dot(glm::normalize(-m_LightDir), normal), 0.0f);

	glm::vec3 sphereColor = closestSphere->Albedo;

	return glm::vec4(sphereColor * intensity, 1.0f);
}
