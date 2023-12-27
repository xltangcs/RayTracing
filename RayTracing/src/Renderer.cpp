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

void Renderer::Render(const Camera& camera)
{
	Ray ray;
	ray.Origin = camera.GetPosition();
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

			glm::vec4 color = TraceRay(ray);

			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::TraceRay(const Ray& ray)
{
	glm::vec3 circle_center(0.0f, 0.0f, 0.0f);
	float r = m_CircleRadius;

	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2 * glm::dot(ray.Direction, ray.Origin - circle_center);
	float c = glm::dot(ray.Origin - circle_center, ray.Origin - circle_center) - r * r;

	float discriminant = b * b - 4.0f * a * c;

	if (discriminant < 0) return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float t0 = (-b - glm::sqrt(discriminant)) / (2.0f * a); //smaller
	float t1 = (-b + glm::sqrt(discriminant)) / (2.0f * a);

	glm::vec3 hitpoint = ray.Origin + ray.Direction * t0;  //first hitpoint

	glm::vec3 normal = glm::normalize(hitpoint); //normal

	float intensity = glm::max(glm::dot(-ray.Direction, normal), 0.0f);

	glm::vec3 sphereColor = glm::vec3(1.0f, 1.0f, 0.0f);

	return glm::vec4(sphereColor * intensity, 1.0f);
}
