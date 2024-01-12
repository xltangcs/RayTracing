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
		:m_Camera(45.0f, 0.1f, 100.0f), CurrentScene(BaseSphereScene), BVH(true)
	{
		BuildMaterial();
		BuildBaseSphere();
		BuildRandomScene();
	}
	void BuildMaterial()
	{
		//0 - 4
		m_Renderer.AddMaterial(std::make_shared<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f)));
		m_Renderer.AddMaterial(std::make_shared<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f)));
		m_Renderer.AddMaterial(std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.0f));
		m_Renderer.AddMaterial(std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f), 1.0f));
		m_Renderer.AddMaterial(std::make_shared<Dielectric>(1.5f));

		//5 - 14
		for (int i = 0; i < 10; i++)
		{
			auto albedo = Toffee::Random::Vec3();
			m_Renderer.AddMaterial(std::make_shared<Lambertian>(albedo));
		}

		//15 - 24
		for (int i = 0; i < 10; i++)
		{
			auto albedo = Toffee::Random::Vec3(0.5f, 1.0f);
			auto roughness = Toffee::Random::Float() * 0.5;
			m_Renderer.AddMaterial(std::make_shared<Metal>(albedo, roughness));
		}
	}

	void BuildBaseSphere()
	{
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, 1));
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(0.0f, 100.5f, -1.0f), 100.0f, 0));
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, 2));
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, 1.0f), 0.5f, 3));
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, 1.0f), 0.5f, 4));
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, 1.0f), -0.45f, 4));
	
		BVHBaseSphere = BVHNode(BaseSphere, 0, 1);
	}

	void BuildRandomScene()
	{
		for (int a = -11; a < 11; a++)
		{
			for (int b = -11; b < 11; b++)
			{
				float chooseMarerial = Toffee::Random::Float();
				glm::vec3 center(a + 0.9f * Toffee::Random::Float(), 0.2f, b + 0.9 * Toffee::Random::Float());
				if ((center - glm::vec3(4.0f, 0.2f, 0.0f)).length() > 0.9f)
				{
					if (chooseMarerial < 0.8f)//diffuse
					{
						int materialIndex = Toffee::Random::UInt(3, 12);
						RandomSphere.AddSceneObject(std::make_shared<Sphere>(center, 0.2f, materialIndex));
					}
					else if (chooseMarerial < 0.95f)//Metal
					{
						int materialIndex = Toffee::Random::UInt(13, 22);
						RandomSphere.AddSceneObject(std::make_shared<Sphere>(center, 0.3f, materialIndex));
					}
					else
					{
						RandomSphere.AddSceneObject(std::make_shared<Sphere>(center, 0.4f, 2));
					}
				}
			}
		}

		RandomSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(0.0f, 1000.0f, 0.0f), 1000.0, 0));

		RandomSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, 2)); //Dielectric
		RandomSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, 1)); //Metal
		RandomSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, 3)); //Lambertian

		BVHRandomSphere = BVHNode(RandomSphere, 0, RandomSphere.GetSceneObjectSize());
	}

	virtual void OnUpdate(float ts) override
	{
		if (m_Camera.OnUpdate(ts))
			m_Renderer.ResetFrameIndex();
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");


		ImGui::Text("The Last Render time: %.3fms", m_LastRenderTime);
		ImGui::Text("The average fps: %.3f", ImGui::GetIO().Framerate);

		ImGui::DragFloat3("Light Dir", glm::value_ptr(m_Renderer.m_LightDir), 0.01f);
		ImGui::InputInt("Bounces", &m_Renderer.m_Bounces);
		ImGui::Checkbox("Accumulate", &m_Renderer.Accumulate);
		ImGui::SameLine();
		ImGui::Checkbox("BVH", &BVH);

		if (ImGui::Button("Reset"))
		{
			m_Renderer.ResetFrameIndex();
		}

		if (ImGui::Button("BaseSphere"))
		{
			CurrentScene = BaseSphereScene;
		}
		ImGui::SameLine();
		if (ImGui::Button("RandomSphere"))
		{
			CurrentScene = RandomSphereScene;
		}

		ImGui::DragFloat3("Camera Position", glm::value_ptr(m_Camera.GetPosition()));


		ImGui::End();

		if (CurrentScene != RandomSphereScene)
		{
			ImGui::Begin("Scene");

			for (int i = 0; i < BaseSphere.GetSceneObjectSize(); i++)
			{
				ImGui::PushID(i);

				std::shared_ptr<Sphere> sphere = std::dynamic_pointer_cast<Sphere>(BaseSphere.GetSceneObject(i));

				ImGui::DragFloat3("Position", glm::value_ptr(sphere->GetPosition()), 0.01f);
				ImGui::DragFloat("Radius", &(sphere->GetRadius()), 0.01f);
				ImGui::SliderInt("Material Index", &sphere->GetMaterialIndex(), 0, (int)m_Renderer.GetMaterialSize());
				ImGui::Separator();

				ImGui::PopID();
			}

			ImGui::End();
		}



		
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

		switch (CurrentScene)
		{
			case BaseSphereScene:
			{
				if(BVH)
					m_Renderer.Render(BVHBaseSphere, m_Camera);
				else
					m_Renderer.Render(BaseSphere, m_Camera);
				break;
			}
			case RandomSphereScene:
			{
				if(BVH)
					m_Renderer.Render(BVHRandomSphere, m_Camera);
				else
					m_Renderer.Render(RandomSphere, m_Camera);
				break;
			}
			default: m_Renderer.Render(BVHBaseSphere, m_Camera); break;
		}

		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	enum SceneClass{
		BaseSphereScene = 0,
		RandomSphereScene = 1
	};


private:
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	Renderer m_Renderer;
	Camera m_Camera;

	SceneClass CurrentScene;
	Scene BaseSphere;
	Scene RandomSphere;
	BVHNode BVHBaseSphere;
	BVHNode BVHRandomSphere;

	bool BVH = true;

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