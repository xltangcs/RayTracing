#pragma once
#include <memory>

#include <glm/glm.hpp>

struct HitPayload
{
	float HitDistance;
	glm::vec3 WorldPosition;
	glm::vec3 WorldNormal;

	glm::vec2 UV;
	int MaterialIndex;

	bool FrontFace;
};