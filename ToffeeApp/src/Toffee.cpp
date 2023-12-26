#include "Toffee/Application.h"
#include "Toffee/EntryPoint.h"

#include "Toffee/Image.h"

class ExampleLayer : public Toffee::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Hello");
		ImGui::Button("Button");
		ImGui::End();

		//ImGui::ShowDemoWindow();
	}
};

Toffee::Application* Toffee::CreateApplication(int argc, char** argv)
{
	Toffee::ApplicationSpecification spec;
	spec.Name = "Toffee Example";

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