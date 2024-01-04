#pragma once

#include <string>

#include <GLFW//glfw3.h>

class Application {
public:
	Application(const std::string& applicationname = "App");
	~Application();

	void Run();

private:
	void InitImGui();
	void shutdown();

private:
	std::string m_ApplicationName;
	GLFWwindow* m_GLFWwindow = nullptr;
	bool m_Running = false;

};