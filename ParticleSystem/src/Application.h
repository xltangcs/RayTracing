#pragma once

#include <string>
#include <vector>
#include <memory>

#include "imgui.h"
#include <GLFW//glfw3.h>

#include "ImGuiLayer.h"

class Application {
public:
	Application(const std::string& applicationname = "App");
	~Application();

	void Run();

	template<typename T>
	void PushImGuiLayer()
	{
		static_assert(std::is_base_of<ImGuiLayer, T>::value, "Pushed type is not subclass of ImGuiLayer!");
		m_ImGuiLayerVector.emplace_back(std::make_shared<T>());
	}

	void Close();
private:
	void InitImGui();
	

private:
	std::string m_ApplicationName;
	GLFWwindow* m_GLFWwindow = nullptr;
	bool m_Running = false;
	std::vector<std::shared_ptr<ImGuiLayer>> m_ImGuiLayerVector;
};