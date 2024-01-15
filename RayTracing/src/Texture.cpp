#include "Texture.h"

#include <iostream>

ConstantTexture::ConstantTexture(glm::vec3 color)
	:m_Color(color)
{
}

glm::vec3 ConstantTexture::GetColor(glm::vec2 uv, const glm::vec3& p) const
{
	return m_Color;
}

CheckerBoardTexture::CheckerBoardTexture(std::shared_ptr<Texture> odd, std::shared_ptr<Texture> even)
	:m_Odd(odd), m_Even(even)
{
}

glm::vec3 CheckerBoardTexture::GetColor(glm::vec2 uv, const glm::vec3& p) const
{
	auto xInteger = static_cast<int>(std::floor(m_TileSize * p.x));
	auto yInteger = static_cast<int>(std::floor(m_TileSize * p.y));
	auto zInteger = static_cast<int>(std::floor(m_TileSize * p.z));

	bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

	return isEven ? m_Even->GetColor(uv, p) : m_Odd->GetColor(uv, p);

	//auto sines = sin(10.0f * uv.x) * sin(10.0f * uv.y) * sin(10.0f * p.z);

	//if (sines < 0.0f)
	//	return m_Odd->GetColor(uv, p);
	//else
	//	return m_Even->GetColor(uv, p);

}

ImageTexture::ImageTexture(const std::string& path)
	:m_FilePath(path), m_Image(path)
{
	
}

glm::vec3 ImageTexture::GetColor(glm::vec2 uv, const glm::vec3& p) const
{
	if (m_Image.GetHeight() <= 0) return glm::vec3(0.0f, 1.0f, 0.0f);

	auto u = static_cast<int>((uv.x) * m_Image.GetWidth());
	auto v = static_cast<int>((1 - uv.y) * m_Image.GetHeight()- 0.001f);
	auto i = glm::clamp(u, 0, (int)m_Image.GetWidth() - 1);
	auto j = glm::clamp(v, 0, (int)m_Image.GetHeight() - 1);

	//auto u = glm::clamp(uv.x, 0.0f, 1.0f);
	//auto v = 1.0f - glm::clamp(uv.y, 0.0f, 1.0f);
	//int i = static_cast<int>(u * m_Image.GetWidth() - 1);
	//int j = static_cast<int>(v * m_Image.GetHeight() - 1);

	auto r = static_cast<float>(m_Image.m_ImageData[3 * i + 3 * m_Image.GetWidth() * j + 0]) / 255.0;
	auto g = static_cast<float>(m_Image.m_ImageData[3 * i + 3 * m_Image.GetWidth() * j + 1]) / 255.0;
	auto b = static_cast<float>(m_Image.m_ImageData[3 * i + 3 * m_Image.GetWidth() * j + 2]) / 255.0;
	
	return glm::vec3(r, g, b);

	//TO DO: load image wrong, try modify vulkan to opengl
	//return m_Image.PixelData(i, j);

	//auto color_scale = 1.0f / 255.0f;
	//return glm::vec3(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}





