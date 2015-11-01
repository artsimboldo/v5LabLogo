#include "stdafx.h"
#include "Scanner.h"
#include <ctype.h>
#include "Utils.h"
#include "LogoException.h"

namespace v5Logo
{
	map<wstring, Token> CScanner::mStringToToken =
	{
		{ L"REPEAT", Token::REPEAT },
		{ L"FORWARD", Token::FORWARD }, { L"FD", Token::FORWARD },
		{ L"BACK", Token::BACK }, { L"BK", Token::BACK },
		{ L"LEFT", Token::LEFT }, { L"LT", Token::LEFT },
		{ L"RIGHT", Token::RIGHT }, { L"RT", Token::RIGHT },
		{ L"UP", Token::UP },
		{ L"DOWN", Token::DOWN }, { L"DN", Token::DOWN },
		{ L"ROLLRIGHT", Token::ROLLRIGHT }, { L"RR", Token::ROLLRIGHT },
		{ L"ROLLLEFT", Token::ROLLLEFT }, { L"RL", Token::ROLLLEFT },
		{ L"HOME", Token::HOME },
		{ L"MOVETO", Token::MOVETO }, { L"GOTO", Token::MOVETO }, { L"SETPOS", Token::MOVETO },
		{ L"END", Token::END },
		{ L"CLEARSCREEN", Token::CLEAR }, { L"CLEAR", Token::CLEAR }, { L"CLS", Token::CLEAR },{ L"CLR", Token::CLEAR },
		{ L"PENUP", Token::PENUP }, { L"PU", Token::PENUP },
		{ L"PENDOWN", Token::PENDOWN }, { L"PD", Token::PENDOWN },
		{ L"PENWIDTH", Token::PENWIDTH }, { L"PW", Token::PENWIDTH }
	};

	CScanner::CScanner(wstring program) : 
		mIdx(0),
		mScanBuffer(L"")
	{
		mProgram = program;
	}

	Token CScanner::NextToken()
	{
		int temp = mIdx;
		Token tok = Scan();
		mIdx = temp;
		return tok;
	}

	Token CScanner::Scan()
	{
		wchar_t ch;
		while (mIdx < mProgram.length())
		{
			ch = mProgram[mIdx];
			switch (ch)
			{
				case L',':
					mIdx++;
					return Token::COMMA;
				default:
					if (isdigit(ch))
					{
						mScanBuffer = ch;
						mIdx++;
						while (mIdx < mProgram.length())
						{
							ch = mProgram[mIdx];
							if (isdigit(ch))
							{
								mScanBuffer += ch;
								mIdx++;
							}
							else break;
						}
						return Token::NUMBER;
					}
					else if (isalnum(ch))
					{
						mScanBuffer = ch;
						mIdx++;
						while (mIdx < mProgram.length())
						{
							ch = mProgram[mIdx];
							if (isalnum(ch) || ch == L'_')
							{
								mScanBuffer += ch;
								mIdx++;
							}
							else break;
						}
						ToUpper(mScanBuffer);
						return LookupReserved(mScanBuffer);
					}
					else if (isspace(ch))
					{
						mIdx++;
					}
					else
					{
						throw CLogoException(wFormat(L"Lexical error! unknowned token '%c' ('%s')\n", ch, mScanBuffer.c_str()));
					}
					break;
			}
		}
		return Token::END_OF_FILE;
	}

	// TODO : O(1) 
	wstring CScanner::GetTokenString(Token token) 
	{
		for(const auto& p : mStringToToken)
		{
			if (p.second == token)
			{
				return p.first;
			}
		}
		throw CLogoException(L"Error: Unknown token string!");
	}

	Token CScanner::LookupReserved(const wstring s)
	{
		for(const auto& p : mStringToToken)
		{
			if (p.first ==  s)
			{
				return p.second;
			}
		}
		return Token::IDENTIFIER;
	}
}