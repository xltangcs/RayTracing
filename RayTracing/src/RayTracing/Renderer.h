#pragma once
#include <memory>

#include <glm/glm.hpp>

#include "Tools/Image.h"

#include "Ray.h"
#include "Camera.h"

#include "Hittable.h"
#include "HittableList.h"


class Renderer {
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render(Camera& camera, Hittable_list& world);

	std::shared_ptr<Image> GetFinalImage() const { return  m_FinalImage; }

private:
	glm::vec3 ray_color(Ray& r);
private:
	Camera* m_Camera = nullptr;
	Hittable_list* m_World = nullptr;
	std::shared_ptr<Image> m_FinalImage = nullptr;
	uint32_t* m_ImageData = nullptr;
};