#pragma once

#include "Toffee/Image.h"

#include <memory>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	std::shared_ptr<Toffee::Image> GetFinalImage() const { return  m_FinalImage; }
	void SetRadius(float radius) { this->m_CircleRadius = radius; }
	void SetLightDir(glm::vec3 LightDir) { this->m_LightDir = LightDir; }

private:
	glm::vec4 PerPixel(glm::vec2 coord);
private:
	std::shared_ptr<Toffee::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

	float m_CircleRadius = 0.5f;
	glm::vec3 m_LightDir = glm::vec3(-1.0f, -1.0f, -1.0f);
};
