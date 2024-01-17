#include <memory>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Tools/Application.h"
#include "Tools/Image.h"
#include "RayTracing/Renderer.h"



class MyImGuiLayer : public ImGuiLayer
{
public:
	MyImGuiLayer()
	{

	}

	virtual void ShowUI(float ts) override
	{
		ImGui::Begin("Image");
		static Image image2("./assets/textures/Checkerboard.png");
		ImGui::Image((void*)image2.GetTextureID(),
			ImVec2{ (float)image2.GetWidth(), (float)image2.GetHeight() },
			ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();


		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		auto image = m_Renderer.GetFinalImage();
		ImGui::Image((ImTextureID)image->GetTextureID(), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});

		ImGui::End();

		

		Render();

	}

	void Render()
	{
		m_Renderer.Render();
	}


	virtual void OnUpdate(float ts) override
	{
		glViewport(0, 0, (GLsizei)m_ViewportSize.x, (GLsizei)m_ViewportSize.y);
		m_Renderer.OnResize(m_ViewportSize.x, m_ViewportSize.y);
	}

private:
	glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	glm::vec2 m_ViewportBounds[2];

	Renderer m_Renderer;
};


int main()
{
	static Application* ParticleSystem = new Application("Ray Tracing");
	std::shared_ptr<MyImGuiLayer> myimguilayer = std::make_shared<MyImGuiLayer>();

	ParticleSystem->PushImGuiLayer(myimguilayer);

	ParticleSystem->Run();

	return 0;
}