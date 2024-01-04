#include <memory>
#include "Application.h"

class MyImGuiLayer : public ImGuiLayer
{
	virtual void ShowUI() override
	{
		ImGui::Begin("hello");
		ImGui::End();
		ImGui::ShowDemoWindow();
	}
};

int main()
{
	Application* ParticleSystem = new Application("Particle System");

	ParticleSystem->PushImGuiLayer<MyImGuiLayer> ();

	ParticleSystem->Run();

	return 0;
}
