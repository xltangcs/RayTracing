#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Material
{
	glm::vec3 Albedo{ 0.6f, 0.7f, 0.8f };
	float Roughness = 1.0f;
	float Metallic = 0.0f;
};

struct Sphere
{
	glm::vec3 Position{ 0.0f };
	float Radius = 0.5f;

	int MaterialIndex = 0;
};

struct Scene
{
	std::vector<Sphere> Spheres;
	std::vector<Material> Materials;
};