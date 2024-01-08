#pragma once

#include <glm/glm.hpp>

class OrthographicCamera
{
public:
	OrthographicCamera(float aspectRatio, float zoomLevel, float zNear = -1.0f, float zFar = 1.0f, bool isrotation = true);

	void OnUpdate(float ts);
	void OnResize(uint32_t width, uint32_t height);


	const glm::mat4& GetProjection() const { return m_Projection; }
	const glm::mat4& GetView() const { return m_View; }

	const glm::vec3& GetPosition() const { return m_Position; }
	float GetRotation() const { return m_Rotation; }

	float& GetZoomLevel() { return m_ZoomLevel; }
	float GetAspectRatio() const { return m_AspectRatio; }


private:
	void RecalculateProjection();
	void RecalculateView();

private:

	float m_zNear = -1.0f, m_zFar = 1.0f;

	float m_AspectRatio;
	float m_ZoomLevel = 1.0f;

	glm::mat4 m_Projection;
	glm::mat4 m_View;

	bool m_isRotation;

	glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	float m_Rotation = 0.0f;
};