#pragma once
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
using namespace std;

class CEntity
{
public:
	CEntity(void) {};
	CEntity(wstring name) { mName = name; };
	virtual ~CEntity(void) {};
	virtual void UpdateEntity() {};

public:
	wstring	mName;
	glm::vec3	mPosition;
	glm::vec3	mRotation;
	glm::fquat mQuat;
	static const glm::vec3 sForward;
	static const glm::vec3 sUp;
	static const glm::vec3 sRight;
};

