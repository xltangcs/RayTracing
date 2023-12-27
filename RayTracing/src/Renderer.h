#pragma once

#include "Toffee/Image.h"

#include <memory>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Scene.h"
#include "Ray.h"

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const Camera &camera);

	std::shared_ptr<Toffee::Image> GetFinalImage() const { return  m_FinalImage; }
	void SetRadius(float radius) { this->m_CircleRadius = radius; }
	void SetLightDir(glm::vec3 lightDir) { this->m_LightDir = lightDir; }

private:
	glm::vec4 TraceRay(const Scene& scene, const Ray& ray);
private:
	std::shared_ptr<Toffee::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

	float m_CircleRadius = 0.5f;
	glm::vec3 m_LightDir = { -1.0f, -1.0f, -1.0f };
};
