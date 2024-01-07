#include <memory>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Renderer.h"
#include "Framebuffer.h"
#include "Application.h"

class MyImGuiLayer : public ImGuiLayer
{
public:
	MyImGuiLayer()
        : m_Framebuffer(100, 80), m_Camera(45.0f, 0.1f, 100.0f)
	{
		
	}
	
	virtual void ShowUI() override
	{

		ImGui::Begin("Viewport");
        // we access the ImGui window size
        float window_width = ImGui::GetContentRegionAvail().x;
        float window_height = ImGui::GetContentRegionAvail().y;

		m_Width = (int) window_width;
		m_Height = (int) window_height;

        ImGui::Image((ImTextureID)m_Framebuffer.GetTextureID(), ImVec2(window_width, window_height), ImVec2 { 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();


		m_Framebuffer.Bind();//Render here
		
		m_Renderer.RenderCube(m_Camera);

		m_Framebuffer.Unbind();

		ImGui::ShowDemoWindow();
	}

	virtual void OnUpdate(float ts) override
	{
		glViewport(0, 0, (GLsizei)m_Width, (GLsizei)m_Height);
		m_Camera.OnUpdate(ts);
		m_Camera.OnResize(m_Width, m_Height);
		m_Framebuffer.Resize(m_Width, m_Height);
	}

private:
	uint32_t m_Width = 100, m_Height = 100;

	Camera m_Camera;
    Framebuffer m_Framebuffer;
	Renderer m_Renderer;
};


int main()
{
	static Application* ParticleSystem = new Application("Particle System");
	std::shared_ptr<MyImGuiLayer> myimguilayer = std::make_shared<MyImGuiLayer>();

	ParticleSystem->PushImGuiLayer(myimguilayer);

	ParticleSystem->Run();

	return 0;
}