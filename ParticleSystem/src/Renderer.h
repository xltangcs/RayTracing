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

private:
	GLuint m_CubeVA = 0;
	std::shared_ptr<Image> m_Image = nullptr;
	std::shared_ptr<Shader> m_CubeShader = nullptr;
};