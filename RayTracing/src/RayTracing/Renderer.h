#pragma once
#include <memory>

#include <glm/glm.hpp>

#include "Tools/Image.h"


class Renderer {
public:
	Renderer() = default;

	void OnResize(uint32_t width, uint32_t height);
	void Render();

	std::shared_ptr<Image> GetFinalImage() const { return  m_FinalImage; }

private:
	std::shared_ptr<Image> m_FinalImage = nullptr;
	uint32_t* m_ImageData = nullptr;
};