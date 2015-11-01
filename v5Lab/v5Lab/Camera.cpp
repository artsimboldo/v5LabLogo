#include "Camera.h"

CCamera::CCamera(void) : mFOV(FIELDOFVIEW)
{
}

CCamera::~CCamera(void)
{
}

void CCamera::Move()
{
	mPosition += mSpeed;
}