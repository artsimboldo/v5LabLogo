#pragma once
using namespace std;

namespace v5Logo
{
	class CParserState
	{
	public:
		CParserState(void) {};
		~CParserState(void) {};
	};

	class CRepeatParserState : public CParserState
	{
	public:
		CRepeatParserState(int count, int idx) { mCounter = count; mIdx = idx; };
		~CRepeatParserState(void) {};
		int mIdx;
		int mCounter;
	};

}
