#pragma once
#include "Entity.h"
#define	NEARPLANE		0.1f
#define	FARPLANE		10000.0f
#define	FIELDOFVIEW		45.f

class CCamera : public CEntity
{
public:
	CCamera(void);
	~CCamera(void);

	void SetFOV(float newFov) { mFOV = newFov; };
	float GetFOV() { return mFOV; };
	void Move();
	void SetSpeed(glm::vec3& newSpeed) { mSpeed = newSpeed; };
	glm::vec3& GetSpeed() { return mSpeed; };

private:
	float	mFOV;
	glm::vec3	mSpeed;
};

