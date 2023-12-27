#pragma once

#include "Toffee/Image.h"

#include <memory>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Ray.h"

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Camera &camera);

	std::shared_ptr<Toffee::Image> GetFinalImage() const { return  m_FinalImage; }
	void SetRadius(float radius) { this->m_CircleRadius = radius; }

private:
	glm::vec4 TraceRay(const Ray& ray);
private:
	std::shared_ptr<Toffee::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

	float m_CircleRadius = 0.5f;
};
