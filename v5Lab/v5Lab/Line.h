#pragma once
#include <glm/vec3.hpp>

class CLine
{
public:
	CLine(void) {};
	CLine(glm::vec3 pos, glm::fquat quat, float width)
	{
		mPos = pos;
		mQuat = quat;
		mWidth = width * 0.5f;
	}
	~CLine(void) {};

public:
	glm::vec3 mPos;
	glm::fquat mQuat;
	float mWidth;
	glm::vec3 mP0, mP1, mP2, mP3;

};
