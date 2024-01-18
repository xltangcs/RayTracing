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
		:m_Camera(45.0f, 0.1f, 100.0f), CurrentScene(NextWeekScene), BVH(true)
	{
		BuildMaterial();
		BuildBaseSphere();
		BuildRandomScene();
		BuildNextWeekScene();
	}

	void BuildMaterial()
	{
		auto blackColor = std::make_shared<ConstantTexture>(glm::vec3(0.1f, 0.1f, 0.1f));
		auto whiteColor = std::make_shared<ConstantTexture>(glm::vec3(0.7f, 0.7f, 0.7f));
		auto redColor = std::make_shared<ConstantTexture>(glm::vec3(0.6f, 0.01f, 0.01f));
		auto yellowColor = std::make_shared<ConstantTexture>(glm::vec3(0.9f, 0.9f, 0.1f));
		auto greenColor = std::make_shared<ConstantTexture>(glm::vec3(0.2f, 0.5f, 0.3f));
		auto lightColor = std::make_shared<ConstantTexture>(glm::vec3(15.0f, 15.0f, 15.0f));

		auto image = std::make_shared<ImageTexture>("assets/textures/Checkerboard.png");

		auto checkerboard = std::make_shared<CheckerBoardTexture>(whiteColor, blackColor);
		//0 - 4
		m_Renderer.AddMaterial(std::make_shared<Lambertian>(checkerboard));
		m_Renderer.AddMaterial(std::make_shared<Lambertian>(redColor));
		m_Renderer.AddMaterial(std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.0f));
		m_Renderer.AddMaterial(std::make_shared<Metal>(glm::vec3(0.5f, 0.8f, 0.5f), 1.0f));
		m_Renderer.AddMaterial(std::make_shared<Dielectric>(1.5f));

		//5 - 14
		for (int i = 0; i < 10; i++)
		{
			auto albedo = Toffee::Random::Vec3();
			auto texture = std::make_shared<ConstantTexture>(albedo);
			m_Renderer.AddMaterial(std::make_shared<Lambertian>(texture));
		}

		//15 - 24
		for (int i = 0; i < 10; i++)
		{
			auto albedo = Toffee::Random::Vec3(0.5f, 1.0f);
			auto roughness = Toffee::Random::Float() * 0.5f;
			m_Renderer.AddMaterial(std::make_shared<Metal>(albedo, roughness));
		}

		//25
		auto diffuselight = std::make_shared<DiffuseLight>(lightColor);
		m_Renderer.AddMaterial(diffuselight);

		{ //26
			auto material = std::make_shared<Lambertian>(redColor);
			m_Renderer.AddMaterial(material);
		}
		{ //27
			auto material = std::make_shared<Lambertian>(greenColor);
			m_Renderer.AddMaterial(material);
		}
		{ //28
			auto material = std::make_shared<Lambertian>(whiteColor);
			m_Renderer.AddMaterial(material);
		}
	}

	void BuildBaseSphere()
	{
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, 1));
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, 0));
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, 2));
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(1.0f, 0.0f, 1.0f), 0.5f, 3));
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, 1.0f), 0.5f, 4));
		BaseSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(-1.0f, 0.0f, 1.0f), -0.45f, 4));
	

		auto quad = std::make_shared<Quad>(glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(4.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 4.0f, 0.0f),
			25);
		BaseSphere.AddSceneObject(quad);

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

		RandomSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(0.0f, 1000.0f, 0.0f), 999.0f, 0));

		RandomSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, 2)); //Dielectric
		RandomSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, 1)); //Metal
		RandomSphere.AddSceneObject(std::make_shared<Sphere>(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, 3)); //Lambertian

		BVHRandomSphere = BVHNode(RandomSphere, 0, RandomSphere.GetSceneObjectSize());
	}

	void BuildNextWeekScene()
	{
		{ //light
			auto quad = std::make_shared<Quad>(glm::vec3(-1.0f, 2.8f, 1.0f),
				glm::vec3(0.0f, 0.0f, 2.0f),
				glm::vec3(2.0f, 0.0f, 0.0f),
				25);
			NextWeek.AddSceneObject(quad);
		}
		{ //right
			auto quad = std::make_shared<Quad>(glm::vec3(3.0f, -5.0f, -2.0f),
				glm::vec3(0.0f, 10.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 10.0f),
				26);
			NextWeek.AddSceneObject(quad);
		}
		{ //left
			auto quad = std::make_shared<Quad>(glm::vec3(-3.0f, -5.0f, -2.0f),
				glm::vec3(0.0f, 10.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 10.0f),
				27);
			NextWeek.AddSceneObject(quad);
		}
		{ //up
			auto quad = std::make_shared<Quad>(glm::vec3(-5.0f, 3.0f, -5.0f),
				glm::vec3(0.0f, 0.0f, 10.0f),
				glm::vec3(10.0f, 0.0f, 0.0f),
				28);
			NextWeek.AddSceneObject(quad);
		}
		{ //down
			auto quad = std::make_shared<Quad>(glm::vec3(-5.0f, -3.0f, -5.0f),
				glm::vec3(0.0f, 0.0f, 10.0f),
				glm::vec3(10.0f, 0.0f, 0.0f),
				28);
			NextWeek.AddSceneObject(quad);
		}
		{ //center
			auto quad = std::make_shared<Quad>(glm::vec3(-5.0f, -5.0f, 0.0f),
				glm::vec3(10.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 10.0f, 0.0f),
				28);
			NextWeek.AddSceneObject(quad);
		}
		NextWeek.AddSceneObject(std::make_shared<Sphere>(glm::vec3(-1.5f, -1.5f, 1.0f), 1.5f, 2));

		{ //cube1
			auto cube = Cube(glm::vec3(1.0f, -2.0f, 1.0f),
				glm::vec3(1.5f, 1.5f, 2.0f),
				28);
			NextWeek.AddSceneObject(cube);
		}


		BVHNextWeek = BVHNode(NextWeek, 0, NextWeek.GetSceneObjectSize());
	}

	std::shared_ptr<Scene> Cube(const glm::vec3& a, const glm::vec3& b, int materialindex)
	{
		auto cube = std::make_shared<Scene>();
		auto min = glm::vec3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
		auto max = glm::vec3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
		auto dx = glm::vec3(max.x - min.x, 0, 0);
		auto dy = glm::vec3(0, max.y - min.y, 0);
		auto dz = glm::vec3(0, 0, max.z - min.z);
		cube->AddSceneObject(std::make_shared<Quad>(glm::vec3(min.x, min.y, max.z), dx, dy, materialindex)); // Ç°
		cube->AddSceneObject(std::make_shared<Quad>(glm::vec3(max.x, min.y, max.z), -dz, dy,materialindex)); // ÓÒ
		cube->AddSceneObject(std::make_shared<Quad>(glm::vec3(max.x, min.y, min.z), -dx, dy,materialindex)); // ºó
		cube->AddSceneObject(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dz, dy, materialindex)); // ×ó
		cube->AddSceneObject(std::make_shared<Quad>(glm::vec3(min.x, max.y, max.z), dx, -dz,materialindex)); // ¶¥
		cube->AddSceneObject(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dx, dz, materialindex)); // µ×
		return cube;
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

		ImGui::InputInt("Bounces", &m_Renderer.m_Bounces);
		ImGui::Checkbox("Accumulate", &m_Renderer.Accumulate);
		ImGui::SameLine();
		ImGui::Checkbox("BVH", &BVH);
		ImGui::SameLine();
		ImGui::Checkbox("Source Light", &m_Renderer.SourceLight);

		if (ImGui::Button("Reset"))
		{
			m_Renderer.ResetFrameIndex();
		}
		ImGui::SameLine();
		if (ImGui::Button("Add Quad"))
		{
			auto quad = std::make_shared<Quad>(glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(4.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 4.0f, 0.0f),
				28);
			NextWeek.AddSceneObject(quad);
		}
		ImGui::SameLine();
		if (ImGui::Button("Add Sphere"))
		{
			NextWeek.AddSceneObject(std::make_shared<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, 1));
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
		ImGui::SameLine();
		if (ImGui::Button("NextWeek"))
		{
			CurrentScene = NextWeekScene;
		}

		ImGui::DragFloat3("Camera Position", glm::value_ptr(m_Camera.GetPosition()));
		ImGui::Checkbox("Camera Rotation", &m_Camera.GetIsRotation());

		ImGui::End();


		if (CurrentScene != RandomSphereScene)
		{
			ImGui::Begin("Scene");
			int SceneObjectSize = 0;
			if (CurrentScene == NextWeekScene)
			{
				SceneObjectSize = NextWeek.GetSceneObjectSize();
			}
			else
			{
				SceneObjectSize = BaseSphere.GetSceneObjectSize();
			}

			for (int i = 0; i < SceneObjectSize; i++)
			{
				ImGui::PushID(i);

				std::shared_ptr<SceneObject> obj = nullptr;
				if (CurrentScene == NextWeekScene)
				{
					obj = NextWeek.GetSceneObject(i);
				}
				else
				{
					obj = BaseSphere.GetSceneObject(i);
				}

				if (obj->m_Type == "Sphere")
				{
					std::shared_ptr<Sphere> sphere = std::dynamic_pointer_cast<Sphere>(obj);
					ImGui::DragFloat3("Position", glm::value_ptr(sphere->GetPosition()), 0.01f);
					ImGui::DragFloat("Radius", &(sphere->GetRadius()), 0.01f);
					ImGui::SliderInt("Material Index", &sphere->m_MaterialIndex, 0, (int)m_Renderer.GetMaterialSize());
				}
				else if (obj->m_Type == "Quad")
				{
					std::shared_ptr<Quad> quad = std::dynamic_pointer_cast<Quad>(obj);
					ImGui::DragFloat3("Position", glm::value_ptr(quad->GetPosition()), 0.01f);
					ImGui::DragFloat3("U Vector", glm::value_ptr(quad->GetUDirection()), 0.01f);
					ImGui::DragFloat3("V Vector", glm::value_ptr(quad->GetVDirection()), 0.01f);
					ImGui::SliderInt("Material Index", &quad->m_MaterialIndex, 0, (int)m_Renderer.GetMaterialSize());
				}
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
			case NextWeekScene:
			{
				if (BVH)
					m_Renderer.Render(BVHNextWeek, m_Camera);
				else
					m_Renderer.Render(NextWeek, m_Camera);
				break;
			}
			default: m_Renderer.Render(BVHBaseSphere, m_Camera); break;
		}

		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	enum SceneClass{
		BaseSphereScene = 0,
		RandomSphereScene = 1,
		NextWeekScene = 2
	};


private:
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	Renderer m_Renderer;
	Camera m_Camera;

	SceneClass CurrentScene;

	Scene BaseSphere;
	Scene RandomSphere;
	Scene NextWeek;

	BVHNode BVHBaseSphere;
	BVHNode BVHRandomSphere;
	BVHNode BVHNextWeek;

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