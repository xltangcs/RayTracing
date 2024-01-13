#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class Texture
{
public:
	virtual glm::vec3 GetColor(glm::vec2 uv, const glm::vec3& p) const = 0;
};

class ConstantTexture : public Texture
{
public:
	ConstantTexture() = default;
	ConstantTexture(glm::vec3 color);

	virtual glm::vec3 GetColor(glm::vec2 uv, const glm::vec3& p) const;

private:
	glm::vec3 m_Color;
};

class CheckerBoardTexture : public Texture
{
public:
	CheckerBoardTexture() = default;
	CheckerBoardTexture(std::shared_ptr<Texture> odd, std::shared_ptr<Texture> even);

	virtual glm::vec3 GetColor(glm::vec2 uv, const glm::vec3& p) const;

private:
	float m_TileSize = 2.0f;
	std::shared_ptr<Texture> m_Odd;
	std::shared_ptr<Texture> m_Even;
};

class ImageTexture : public Texture
{
public:
	ImageTexture() = default;
	ImageTexture(const std::string& path);

	virtual glm::vec3 GetColor(glm::vec2 uv, const glm::vec3& p) const;

	void LoadImage(const std::string& path);
	const unsigned char* PixelData(int x, int y) const;

	uint32_t& GetWidth() { return m_ImageWidth; }
	uint32_t& GetHeight() { return m_ImageHeight; }


private:
	std::string m_FilePath;
	unsigned char* m_ImageData = nullptr;
	uint32_t m_ImageWidth, m_ImageHeight;
	int m_nrComponents;
};