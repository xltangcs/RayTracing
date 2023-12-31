#pragma once

#include "Toffee/Image.h"
#include "Toffee/Random.h"

#include <memory>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Scene.h"
#include "Ray.h"

class Renderer
{
public:
	bool Accumulate = true;

	glm::vec3 m_LightDir = { -1.0f, 0.5f, -1.0f };
	int m_Bounces = 2;
public:
	Renderer() = default;
	~Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const Camera &camera);

	std::shared_ptr<Toffee::Image> GetFinalImage() const { return  m_FinalImage; }

	void ResetFrameIndex() { m_FrameIndex = 1; }
private:
	struct HitPayload
	{
		float HitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;

		int ObjectIndex;
	};

private:
	glm::vec4 PerPixel(uint32_t x, uint32_t y); // RayGen
	HitPayload TraceRay(const Ray& ray);
	HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
	HitPayload Miss(const Ray& ray);
private:
	std::shared_ptr<Toffee::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	glm::vec4* m_AccumulationData = nullptr;

	uint32_t m_FrameIndex = 1;

	std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

	const Camera* m_ActiveCamera = nullptr;
	const Scene* m_ActiveScene = nullptr;
};
