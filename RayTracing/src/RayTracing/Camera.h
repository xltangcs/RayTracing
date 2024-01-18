#pragma once

#include <vector>
#include <glm/glm.hpp>

class Camera
{
public:
	Camera(glm::vec3 position, float aspectRatio);

	void OnUpdate(float ts);
	void OnResize(uint32_t width, uint32_t height);

	const glm::vec3& GetPosition() const { return m_Position; }
	float GetAspectRatio() const { return m_AspectRatio; }
	const std::vector<glm::vec3>& GetRayDirections() const { return m_RayDirections; }
private:
	void RecalculateRayDirections();

private:
	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	float m_AspectRatio;

	std::vector<glm::vec3> m_RayDirections;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
};