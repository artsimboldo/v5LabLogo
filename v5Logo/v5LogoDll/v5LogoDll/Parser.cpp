#include "stdafx.h"
#include "Parser.h"
#include "Utils.h"

namespace v5Logo
{
	CParser::CParser()
	{
		Default();
	}

	CParser::CParser(const CParser &parser)
	{
		// TODO
	}

	CParser::~CParser(void) { }

	void CParser::Default()
	{
		mDistance = 1.f;
		mX = mY = mZ = 0.f;
		mPitch = mYaw = mRoll = 1.f;
		mPenState = false;
		mPenWidth = 1.f;
	}

	//
	Token CParser::Parse()
	{
		Token nextToken = mpScanner->NextToken();
		switch (nextToken)
		{
			// <logo-clear-command> ::= CLEAR
		case Token::HOME:
			Match(Token::HOME);
			Default();
			break;
			// <logo-move-command> ::= MOVETO <integer> , <integer>
		case Token::MOVETO:
			Match(Token::MOVETO);
			Match(Token::NUMBER);
			mX = (float)MatchNumber(mpScanner->GetScanBuffer());
			Match(Token::COMMA);
			Match(Token::NUMBER);
			mY = (float)MatchNumber(mpScanner->GetScanBuffer());
			Match(Token::COMMA);
			Match(Token::NUMBER);
			mZ = (float)MatchNumber(mpScanner->GetScanBuffer());
			break;
			// <logo-direction-command> ::= FORWARD <integer> | BACK <integer>
		case Token::FORWARD:
			Match(Token::FORWARD);
			if (Try(Token::NUMBER))
			{
				Match(Token::NUMBER);
				mDistance = (float)MatchNumber(mpScanner->GetScanBuffer());
			}
			else
			{
				mDistance = fabs(mDistance);
			}
			break;
		case Token::BACK:
			Match(Token::BACK);
			if (Try(Token::NUMBER))
			{
				Match(Token::NUMBER);
				mDistance = -(float)MatchNumber(mpScanner->GetScanBuffer());
			}
			else
			{
				mDistance = -fabs(mDistance);
			}
			break;
			// <logo-direction-command> ::= LEFT <integer> | RIGHT <integer>
		case Token::LEFT:
			Match(Token::LEFT);
			if (Try(Token::NUMBER))
			{
				Match(Token::NUMBER);
				mYaw = (float)MatchNumber(mpScanner->GetScanBuffer());
			}
			else
			{
				mYaw = fabs(mYaw);
			}
			break;
		case Token::RIGHT:
			Match(Token::RIGHT);
			if (Try(Token::NUMBER))
			{
				Match(Token::NUMBER);
				mYaw = -(float)MatchNumber(mpScanner->GetScanBuffer());
			}
			else
			{
				mYaw = -fabs(mYaw);
			}
			break;
			// <logo-direction-command> ::= UP <integer> | DOWN <integer>
		case Token::DOWN:
			Match(Token::DOWN);
			if (Try(Token::NUMBER))
			{
				Match(Token::NUMBER);
				mPitch = (float)MatchNumber(mpScanner->GetScanBuffer());
			}
			else
			{
				mPitch = fabs(mPitch);
			}
			break;
		case Token::UP:
			Match(Token::UP);
			if (Try(Token::NUMBER))
			{
				Match(Token::NUMBER);
				mPitch = -(float)MatchNumber(mpScanner->GetScanBuffer());
			}
			else
			{
				mPitch = -fabs(mPitch);
			}
			break;
			// <logo-direction-command> ::= ROLLRIGHT <integer> | ROLLLEFT <integer>
		case Token::ROLLRIGHT:
			Match(Token::ROLLRIGHT);
			if (Try(Token::NUMBER))
			{
				Match(Token::NUMBER);
				mRoll = (float)MatchNumber(mpScanner->GetScanBuffer());
			}
			else
			{
				mRoll = fabs(mRoll);
			}
			break;
		case Token::ROLLLEFT:
			Match(Token::ROLLLEFT);
			if (Try(Token::NUMBER))
			{
				Match(Token::NUMBER);
				mRoll = -(float)MatchNumber(mpScanner->GetScanBuffer());
			}
			else
			{
				mRoll = -fabs(mRoll);
			}
			break;
			// <logo-repeat-command> ::= REPEAT <integer> {<logo-sentence>} END
		case Token::REPEAT:
			Match(Token::REPEAT);
			Match(Token::NUMBER);
			mStates.push_back(unique_ptr<CParserState>(new CRepeatParserState(
				MatchNumber(mpScanner->GetScanBuffer()),
				mpScanner->GetIdx())));
			return Parse();
		case Token::END:
		{
			if (!mStates.empty())
			{
				Match(Token::END);
				CRepeatParserState* state = static_cast<CRepeatParserState*>(mStates.back().get());
				state->mCounter--;
				if (0 == state->mCounter)
				{
					// cancel state
					mStates.pop_back();
				}
				else
				{
					// reset scanner index
					mpScanner->SetIdx(state->mIdx);
				}
				return Parse();
			}
			else
			{
				throw CLogoParserStateException(L"Error! states empty, REPEAT missing?");
			}
		}
		break;
		// <logo-pen-command> ::= CLEAR
		case Token::CLEAR:
			Match(Token::CLEAR);
			break;
			// <logo-pen-command> ::= PENUP | PENDOWN
		case Token::PENUP:
			Match(Token::PENUP);
			mPenState = false;
			break;
		case Token::PENDOWN:
			Match(Token::PENDOWN);
			mPenState = true;
			break;
		case Token::PENWIDTH:
			Match(Token::PENWIDTH);
			Match(Token::NUMBER);
			mPenWidth = (float)MatchNumber(mpScanner->GetScanBuffer());
			break;
			// <logo-end-of-file> ::= IDENTIFIER
		case Token::IDENTIFIER:
			Match(Token::IDENTIFIER);
			break;
			// <logo-end-of-file> ::= EOF
		case Token::END_OF_FILE:
			if (!mStates.empty())
			{
				throw CLogoParserStateException(L"Error! uncomplete state, END missing?");
			}
			break;
		default:
			throw CLogoMatchException(wFormat(L"Error! expected one of: CLEAR, MOVETO, FORWARD, BACK, LEFT, RIGHT, REPEAT or IDENTIFIER but found %s",
				mpScanner->GetTokenString(nextToken).c_str()));
		}
		return nextToken;
	}

	bool CParser::IsReservedToken(const Token token)
	{
		return token < Token::END_OF_RESERVED;
	}

	bool CParser::Try(const Token token)
	{
		return (mpScanner->NextToken() == token);
	}

	void CParser::Match(const Token token)
	{
		Token nextToken = mpScanner->Scan();
		if (nextToken != token)
		{
			throw CLogoMatchException(wFormat(L"Expected %s but found %s (at '%s')",
				mpScanner->GetTokenString(token).c_str(),
				mpScanner->GetTokenString(nextToken).c_str(),
				mpScanner->GetScanBuffer().c_str()));
		}
	}

	int CParser::MatchNumber(const wstring str, const int base)
	{
		wchar_t const* s = str.c_str();
		wchar_t* end;
		long  l;
		errno = 0;
		l = wcstol(s, &end, base);
		if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX)
		{
			throw CLogoNumberException(CLogoNumberException::ErrorType::NumberErrorOverflow,
				wFormat(L"Number overflow error '%s'", s));
		}
		if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN)
		{
			throw CLogoNumberException(CLogoNumberException::ErrorType::NumberErrorUnderflow,
				wFormat(L"Number underflow error '%s'", s));
		}
		if (*s == '\0' || *end != '\0')
		{
			throw CLogoNumberException(CLogoNumberException::ErrorType::NumberErrorInconvertible,
				wFormat(L"Number inconvertible error '%s'", s));
		}
		return l;
	}
}