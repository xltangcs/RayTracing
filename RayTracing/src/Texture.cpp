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
	:m_FilePath(path)
{
	LoadImage(path);
}

glm::vec3 ImageTexture::GetColor(glm::vec2 uv, const glm::vec3& p) const
{
	if (m_ImageHeight <= 0) return glm::vec3(0.0f, 1.0f, 1.0f);
	auto u = glm::clamp(uv.x, 0.0f, 1.0f);
	auto v = 1.0f - glm::clamp(uv.y, 0.0f, 1.0f);

	auto i = static_cast<int>(u * m_ImageWidth);
	auto j = static_cast<int>(v * m_ImageHeight);

	auto pixel = PixelData(i, j);

	auto color_scale = 1.0f / 255.0f;
	return glm::vec3(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
}

void ImageTexture::LoadImage(const std::string& path)
{
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		m_ImageData = data;
		m_ImageWidth = width;
		m_ImageHeight = height;
		m_nrComponents = nrComponents;

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
}

const unsigned char* ImageTexture::PixelData(int x, int y) const
{
	static unsigned char magenta[] = { 255, 0, 255 };
	if (m_ImageData == nullptr) return magenta;

	x = glm::clamp(x, 0, (int)m_ImageWidth);
	y = glm::clamp(y, 0, (int)m_ImageHeight);

	return m_ImageData + y * m_nrComponents * m_ImageWidth + x * m_nrComponents;
}


