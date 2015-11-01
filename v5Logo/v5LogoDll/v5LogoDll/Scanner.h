#pragma once
#include <string>
#include "Tokens.h"
#include <map>
using namespace std;

namespace v5Logo
{
	class CScanner
	{
	public:
		CScanner(wstring = L"");
		CScanner(const CScanner &scanner) {};
		~CScanner(void) {};
		Token NextToken();
		Token Scan();
		wstring GetScanBuffer() { return mScanBuffer; };
		wstring GetTokenString(Token);
		unsigned int GetIdx() { return mIdx; };
		void SetIdx(unsigned int idx) { mIdx = idx; };

		void SetProgram(const wstring program) 
		{
			mIdx = 0;
			mScanBuffer = L"";
			mProgram = program; 
		};

	private:
		unsigned int mIdx;
		wstring mProgram;
		wstring mScanBuffer;
		static map<wstring, Token> mStringToToken;
		Token LookupReserved(const wstring);
	};
}
