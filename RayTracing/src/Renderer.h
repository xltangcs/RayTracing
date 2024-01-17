#pragma once

#include "Toffee/Image.h"
#include "Toffee/Random.h"

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Scene.h"
#include "Material.h"
#include "SceneObject.h"


class Renderer
{
public:
	bool Accumulate = true;
	bool SourceLight = false;
	int m_Bounces = 5;
public:
	Renderer() = default;
	~Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(SceneObject& scene, Camera &camera);

	void AddMaterial(std::shared_ptr<Material> material);
	std::shared_ptr<Material> GetMaterial(int materialIndex);

	std::shared_ptr<Toffee::Image> GetFinalImage() const { return  m_FinalImage; }
	const size_t GetMaterialSize() const { return m_Materials.size(); }

	void ResetFrameIndex() { m_FrameIndex = 1; }
private:
	glm::vec3 Renderer::ray_color(Ray& ray);
	glm::vec3 Renderer::ray_color(const Ray& ray, int depth);
private:
	std::shared_ptr<Toffee::Image> m_FinalImage;

	uint32_t* m_ImageData = nullptr;
	glm::vec3* m_AccumulationData = nullptr;

	uint32_t m_FrameIndex = 1;

	std::vector<uint32_t> m_ImageHorizontalIter, m_ImageVerticalIter;

	Camera* m_ActiveCamera = nullptr;
	SceneObject*  m_ActiveScene  = nullptr;

	std::vector<std::shared_ptr<Material>> m_Materials;
};
