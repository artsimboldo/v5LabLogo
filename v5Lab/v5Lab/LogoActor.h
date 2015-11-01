#pragma once
#include "Entity.h"
#include "v5LogoDll.h"
#include <vector>
#include "Line.h"
using namespace v5Logo;
using namespace std;

class CLogoActor : public CEntity, public CLogoEngine
{
public:
	CLogoActor(void);
	~CLogoActor(void);
	vector<wstring> mProgram;
	vector<unique_ptr<CLine>> mTrace;
	// override Load to reset Token
	void Load(const wstring program) override
	{
		CLogoEngine::Load(program);
		mCurrentToken = Token::EXECUTE;
	};

protected:
	virtual void UpdateEntity();
	// LogoActor State
	Token mCurrentToken;
	glm::vec3 mTargetPosition;
};