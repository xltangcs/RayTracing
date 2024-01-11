#pragma once

#include "Toffee/Image.h"
#include "Toffee/Random.h"

#include <memory>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Scene.h"
#include "Material.h"
#include "SceneObject.h"


class Renderer
{
public:
	bool Accumulate = true;

	glm::vec3 m_LightDir = { -1.0f, 0.5f, -1.0f };
	int m_Bounces = 5;
public:
	Renderer() = default;
	~Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(Scene& scene, const Camera &camera);

	std::shared_ptr<Toffee::Image> GetFinalImage() const { return  m_FinalImage; }

	void ResetFrameIndex() { m_FrameIndex = 1; }
private:
	glm::vec3 Renderer::ray_color(const Ray& ray, int depth);
private:
	std::shared_ptr<Toffee::Image> m_FinalImage;

	uint32_t* m_ImageData = nullptr;
	glm::vec3* m_AccumulationData = nullptr;

	uint32_t m_FrameIndex = 1;

	std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

	const Camera* m_ActiveCamera = nullptr;
	Scene*  m_ActiveScene  = nullptr;
};
