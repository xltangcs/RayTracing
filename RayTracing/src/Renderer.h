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
	void SetRadius(float radius) { this->CircleRadius = radius; }

private:
	uint32_t PerPixel(glm::vec2 coord);
private:
	std::shared_ptr<Toffee::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

	float CircleRadius = 1.0f;
};
