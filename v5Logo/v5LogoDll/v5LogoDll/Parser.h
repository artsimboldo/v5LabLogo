#pragma once
#define DllExport   __declspec( dllexport )
#include "Scanner.h"
#include "ParserState.h"
#include "LogoException.h"
#include <vector>
#include <memory>

namespace v5Logo
{
	class DllExport CParser
	{
	public:
		CParser();
		CParser(const CParser &parser);
		~CParser(void);
		virtual Token Parse();

	protected:
		virtual void Default();

	#pragma warning(disable:4251)
		unique_ptr<CScanner>	mpScanner;			// Scanner
		vector<unique_ptr<CParserState>> mStates;	// Parser states
		// Turtle state -> TODO: encapsulate in a struct?
		float	mDistance;
		float	mX, mY, mZ;
		float	mPitch, mYaw, mRoll;
		bool	mPenState;
		float	mPenWidth;

	private:
		bool	IsReservedToken(const Token);
		void	Match(const Token);
		bool	Try(const Token);
		int		MatchNumber(const wstring, const int base = 10);
	};
}
