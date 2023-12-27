/*********** Toffee **************/
#include "Toffee/Application.h"
#include "Toffee/EntryPoint.h"
#include "Toffee/Image.h"
#include "Toffee/Timer.h"
#include "Toffee/Random.h"
/*********************************/
#include "Renderer.h"
#include "Camera.h"
#include "Scene.h"
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Toffee::Layer
{
public:
	ExampleLayer()
		:m_Camera(45.0f, 0.1f, 100.0f)
	{
		Material& pinkSphere = m_Scene.Materials.emplace_back();
		pinkSphere.Albedo = { 1.0f, 0.0f, 1.0f };
		pinkSphere.Roughness = 0.0f;

		Material& blueSphere = m_Scene.Materials.emplace_back();
		blueSphere.Albedo = { 0.2f, 0.3f, 1.0f };
		blueSphere.Roughness = 0.1f;

		{
			Sphere sphere;
			sphere.Position = { 0.0f, 0.0f, 0.0f };
			sphere.Radius = 0.5f;
			sphere.MaterialIndex = 0;
			m_Scene.Spheres.push_back(sphere);
		}

		{
			Sphere sphere;
			sphere.Position = { 1.0f, 100.5f, -5.0f };
			sphere.Radius = 100.0f;
			sphere.MaterialIndex = 1;
			m_Scene.Spheres.push_back(sphere);
		}
	}

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");

		if (ImGui::Button("Add Sphere"))
		{
			Sphere sphere;
			m_Scene.Spheres.push_back(sphere);
		}

		ImGui::Text("The Last Render time: %.3fms", m_LastRenderTime);
		ImGui::Text("The average fps: %.3f", ImGui::GetIO().Framerate);

		static glm::vec3 lightDir = { -1.0f, 0.5f, -1.0f };
		ImGui::DragFloat3("Light Dir", glm::value_ptr(lightDir), 0.01f);
		m_Renderer.SetLightDir(lightDir);

		static int bounces = 2;
		ImGui::InputInt("Bounces", &bounces);
		m_Renderer.SetBounces(bounces);

		ImGui::End();

		ImGui::Begin("Scene");

		for (int i = 0; i < m_Scene.Spheres.size();i++)
		{
			ImGui::PushID(i);

			Sphere& sphere = m_Scene.Spheres[i];
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.01f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.01f);
			ImGui::InputInt("Material Index", &sphere.MaterialIndex);
			ImGui::Separator();

			ImGui::PopID();
		}

		for (size_t i = 0; i < m_Scene.Materials.size(); i++)
		{
			ImGui::PushID(i);

			Material& material = m_Scene.Materials[i];
			ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
			ImGui::DragFloat("Roughness", &material.Roughness, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic", &material.Metallic, 0.01f, 0.0f, 1.0f);

			ImGui::Separator();

			ImGui::PopID();
		}


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
		m_Renderer.Render(m_Scene, m_Camera);
		m_LastRenderTime = timer.ElapsedMillis();
	}


private:
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	Renderer m_Renderer;
	Camera m_Camera;
	Scene m_Scene;
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