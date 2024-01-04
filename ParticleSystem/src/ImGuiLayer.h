#pragma once

#include <string>

class ImGuiLayer
{
public:
	ImGuiLayer() = default;
	virtual~ImGuiLayer() = default;

	virtual void ShowUI() {};

private:
};