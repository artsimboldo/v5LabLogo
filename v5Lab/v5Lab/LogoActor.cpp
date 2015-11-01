#include "LogoActor.h"
#include "Utils.h"

CLogoActor::CLogoActor(void) : 
	mCurrentToken(Token::EXECUTE)
{
	mProgram.push_back(L"");
}

CLogoActor::~CLogoActor(void) {}

/*
*/
void CLogoActor::UpdateEntity()
{
	switch(mCurrentToken)
	{
	case Token::HOME:
		mPosition = glm::vec3(0.f);
		mTargetPosition = glm::vec3(0.f);
		mQuat = glm::quat();
		mCurrentToken = Token::EXECUTE;
		break;
	case Token::FORWARD:
	case Token::BACK:
		{
			mTargetPosition = mPosition + glm::rotate(mQuat, glm::vec3(CEntity::sForward * mDistance));
			if (mPenState)
			{
				CLine* pLine = new CLine(mPosition, mQuat, mPenWidth / 10.f);
				// Get size and quat from previous line - if exists
				float width = mTrace.empty() ? mPenWidth / 10.f : mTrace.back()->mWidth;
				glm::fquat quat = mTrace.empty() ? mQuat : mTrace.back()->mQuat;
				pLine->mP3 = pLine->mP0 = mPosition + glm::rotate(quat, glm::vec3(-width, 0.f, 0.f));
				pLine->mP2 = pLine->mP1 = mPosition + glm::rotate(quat, glm::vec3(width, 0.f, 0.f));
				mTrace.push_back(unique_ptr<CLine>(pLine));
			}
			mCurrentToken = Token::NONE;
		}
		break;
	case Token::MOVETO:
		mPosition = glm::vec3(mX, mY, mZ);
		mCurrentToken = Token::EXECUTE;
		break;
	case Token::LEFT:
	case Token::RIGHT:
		mQuat *= glm::angleAxis(glm::radians(mYaw), CEntity::sUp);
		mCurrentToken = Token::NONE;
		break;
	case Token::UP:
	case Token::DOWN:
		mQuat *= glm::angleAxis(glm::radians(mPitch), CEntity::sRight);
		mCurrentToken = Token::NONE;
		break;
	case Token::ROLLRIGHT:
	case Token::ROLLLEFT:
		mQuat *= glm::angleAxis(glm::radians(mRoll), CEntity::sForward);
		mCurrentToken = Token::NONE;
		break;
	case Token::CLEAR:
		mTrace.clear();
		mCurrentToken = Token::EXECUTE;
		break;
	case Token::PENUP:
	case Token::PENDOWN:
	case Token::PENWIDTH:
		mCurrentToken = Token::EXECUTE;
		break;
	case Token::END_OF_FILE:
	case Token::PARSE_ERROR:
		break;
	case Token::EXECUTE:
		mCurrentToken = CLogoEngine::Execute();
		break;

#define LOGOACTORSPEEDFACTOR 1.f
#define LOGOACTOREPSILON 0.01f
	default:
		{
			glm::vec3 dir = mTargetPosition - mPosition;
			if (glm::length(dir) < LOGOACTOREPSILON)
			{
				mCurrentToken = Token::EXECUTE;
			}
			else
			{
				mPosition += glm::normalize(dir) * LOGOACTORSPEEDFACTOR;
				if (mPenState)
				{
					glm::vec3 pos = mTrace.back()->mPos;
					float distance = glm::length(mPosition - pos);
					mTrace.back()->mP2 = pos + glm::rotate(mQuat, glm::vec3(mTrace.back()->mWidth, 0.f, distance));
					mTrace.back()->mP3 = pos + glm::rotate(mQuat, glm::vec3(-mTrace.back()->mWidth, 0.f, distance));
				}
			}
		}
		break;
	}
}