#pragma once
#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Image.h"
#include "Shader.h"
#include "Camera.h"

class Renderer {
public:
	Renderer() = default;
	~Renderer();
	void RenderCube(Camera& camera);
	void Resize(uint32_t width, uint32_t height);

	uint32_t GetWidth() { return m_Width; }
	uint32_t GetHeight() { return m_Height; }

private:
	uint32_t m_Width = 100, m_Height = 80;
	GLuint m_CubeVA = 0;
	std::shared_ptr<Image> m_Image = nullptr;
	std::shared_ptr<Shader> m_CubeShader = nullptr;
};