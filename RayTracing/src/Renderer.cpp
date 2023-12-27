#include "Renderer.h"

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

void Renderer::Render()
{
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f;
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	glm::vec3 ray_origin(0.0f, 0.0f, 2.0f);
	glm::vec3 ray_direction(coord.x, coord.y, -1.0f);
	glm::vec3 circle_center(0.0f, 0.0f, 0.0f);
	float r = CircleRadius;

	float a = glm::dot(ray_direction, ray_direction);
	float b = 2 * glm::dot(ray_direction, ray_origin - circle_center);
	float c = glm::dot(ray_origin - circle_center, ray_origin - circle_center) - r * r;

	if ((b * b - 4.0f * a * c) < 0) return 0xff000000;
	else return 0xff00ffff;
}
