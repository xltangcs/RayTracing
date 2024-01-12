#pragma once

#include <glm/glm.hpp>

struct Ray
{
	glm::vec3 Origin;
	glm::vec3 Direction;
	float m_Time;


	Ray() = default;
	Ray(glm::vec3 origin, glm::vec3 direction, float time = 0.0f)
		:Origin(origin), Direction(direction), m_Time(time)
	{

	}

};