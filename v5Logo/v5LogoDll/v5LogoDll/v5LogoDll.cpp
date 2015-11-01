#include "stdafx.h"
#include "v5LogoDll.h"
#include "LogoException.h"
#include "Utils.h"
// Memory leaks detection
#ifdef _DEBUG
#include <vld.h>
#endif  
// _DEBUG

namespace v5Logo
{
	CLogoEngine::CLogoEngine(void)
	{
		mpScanner = unique_ptr<CScanner>(new CScanner()); // !!!: no variadic in VS 2012
	}

	CLogoEngine::~CLogoEngine(void) { }

	void CLogoEngine::Load(const wstring program)
	{
		mpScanner->SetProgram(program);
		mStates.clear();
	}

	Token CLogoEngine::Execute()
	{
		try
		{
			mErrorMessage.clear();
			return Parse();
		}
		catch (CLogoException ex)
		{
			mErrorMessage = ex.what();
			return Token::PARSE_ERROR;
		}
	}
}