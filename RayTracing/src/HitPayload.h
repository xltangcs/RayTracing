#pragma once
#include <memory>

#include <glm/glm.hpp>

struct HitPayload
{
	float HitDistance;
	glm::vec3 WorldPosition;
	glm::vec3 WorldNormal;

	int MaterialIndex;

	bool FrontFace;
};