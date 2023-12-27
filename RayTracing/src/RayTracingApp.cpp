/*********** Toffee **************/
#include "Toffee/Application.h"
#include "Toffee/EntryPoint.h"
#include "Toffee/Image.h"
#include "Toffee/Timer.h"
#include "Toffee/Random.h"
/*********************************/
#include "Renderer.h"
#include "Camera.h"

class ExampleLayer : public Toffee::Layer
{
public:
	ExampleLayer()
		:m_Camera(45.0f, 0.1f, 100.0f) {}

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		if (ImGui::Button("Render"))
		{
			Render();
		}

		ImGui::Text("The Last Render time: %.3fms", m_LastRenderTime);
		ImGui::Text("The average fps: %.3f", ImGui::GetIO().Framerate);

		static float radius = 0.5f;
		ImGui::DragFloat("Radius", &radius, 0.01f, 0.0f, 1.0f);

		//static float lightDir[3] = { -1.0f, -1.0f, -1.0f };
		//static glm::vec3 lightDir = { -1.0f, -1.0f, -1.0f };
		//ImGui::DragFloat3("LightDir", &lightDir[0], 0.01f, -1.0f, 1.0f);
		//m_Renderer.SetLightDir({ lightDir[0], lightDir[1], lightDir[2]});

		m_Renderer.SetRadius(radius);

		ImGui::End();
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
	
		m_ViewportWidth  = (uint32_t)ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = (uint32_t)ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetFinalImage();

		if (image)
		{
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight()});
		}


		ImGui::End();
		ImGui::PopStyleVar();
		Render();
	}

	void Render()
	{
		Toffee::Timer timer;
		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Camera);
		m_LastRenderTime = timer.ElapsedMillis();
	}


private:
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	Renderer m_Renderer;
	Camera m_Camera;
	float m_LastRenderTime = 0.0f;

};

Toffee::Application* Toffee::CreateApplication(int argc, char** argv)
{
	Toffee::ApplicationSpecification spec;
	spec.Name = "RayTracing";

	Toffee::Application* app = new Toffee::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}