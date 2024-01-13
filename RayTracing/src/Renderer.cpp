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
	delete[] m_AccumulationData;
	m_AccumulationData = new glm::vec3[width * height];

	m_ImageHorizontalIter.resize(width);
	m_ImageVerticalIter.resize(height);
	for (uint32_t i = 0; i < width; i++)
		m_ImageHorizontalIter[i] = i;
	for (uint32_t i = 0; i < height; i++)
		m_ImageVerticalIter[i] = i;
}

void Renderer::Render(SceneObject& scene, Camera& camera)
{
	m_ActiveCamera = &camera;
	m_ActiveScene = &scene;

	if (m_FrameIndex == 1)
		memset(m_AccumulationData, 0, (size_t)m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(glm::vec3));
#define MT 0
#ifdef  MT
	std::for_each(std::execution::par, m_ImageVerticalIter.begin(), m_ImageVerticalIter.end(),
		[this](uint32_t y)
		{
			std::for_each(std::execution::par, m_ImageHorizontalIter.begin(), m_ImageHorizontalIter.end(),
			[this, y](uint32_t x)
				{
					Ray ray(m_ActiveCamera->GetPosition(), m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()]);

					glm::vec3 color = ray_color(ray, m_Bounces);
					m_AccumulationData[x + y * m_FinalImage->GetWidth()] += color;

					glm::vec3 accumulatedColor = m_AccumulationData[x + y * m_FinalImage->GetWidth()];
					accumulatedColor /= (float)m_FrameIndex;
					accumulatedColor = glm::clamp(accumulatedColor, glm::vec3(0.0f), glm::vec3(1.0f));
					m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(glm::vec4(accumulatedColor, 1.0f));
				});
		});
#elif
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			Ray ray( m_ActiveCamera->GetPosition(), m_ActiveCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()]);
			
			glm::vec3 color = ray_color(ray, m_Bounces);
			m_AccumulationData[x + y * m_FinalImage->GetWidth()] += color;

			glm::vec3 accumulatedColor = m_AccumulationData[x + y * m_FinalImage->GetWidth()];
			accumulatedColor /= (float)m_FrameIndex;
			accumulatedColor = glm::clamp(accumulatedColor, glm::vec3(0.0f), glm::vec3(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(glm::vec4(accumulatedColor, 1.0f));
		}
	}
#endif

	m_FinalImage->SetData(m_ImageData);
	if (Accumulate)
		m_FrameIndex++;
	else
		m_FrameIndex = 1;
}

void Renderer::AddMaterial(std::shared_ptr<Material> material)
{
	m_Materials.push_back(material);
}

std::shared_ptr<Material> Renderer::GetMaterial(int materialIndex)
{
	if (materialIndex < m_Materials.size()) {
		return m_Materials[materialIndex];
	}
	else {
		return nullptr;
	}
}

glm::vec3 Renderer::ray_color(const Ray& ray, int depth)
{
	if (depth <= 0)
		return glm::vec3(0.0f, 0.0f, 0.0f);

	HitPayload payload;
	if (m_ActiveScene->Hit(ray, 0.001f, std::numeric_limits<float>::max(), payload))
	{
		Ray newray;
		glm::vec3 color;

		if (GetMaterial(payload.MaterialIndex)->Scatter(ray, payload, color, newray))
		{
			return color * ray_color(newray, depth - 1);
		}

		return glm::vec3(0.0f);
	}

	glm::vec3 unit_direction = glm::normalize(ray.Direction);
	float t = 0.5f * (unit_direction.y + 1.0f);
	return glm::vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + t * glm::vec3(0.5f, 0.7f, 1.0f);
}
