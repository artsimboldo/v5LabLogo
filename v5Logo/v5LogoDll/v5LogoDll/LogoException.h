#pragma once
#include <exception>
#include <string>
using namespace std;

namespace v5Logo
{
	class CLogoException : public exception
	{
	public:
		CLogoException(wstring errorString) { m_errorString = errorString; };
		~CLogoException(void) {};
		const wchar_t* what() { return m_errorString.c_str(); };

	private:
		wstring m_errorString;
	};

	class CLogoMatchException : public CLogoException
	{
	public:
		CLogoMatchException(wstring errorString) : CLogoException(errorString) {};
	};

	class CLogoNumberException : public CLogoException
	{
	public:
		enum class ErrorType
		{
			NumberErrorOverflow,
			NumberErrorUnderflow,
			NumberErrorInconvertible,
		};

		CLogoNumberException(ErrorType errorType, wstring errorString) : CLogoException(errorString) 
		{
			m_errorType = errorType;
		};

	private:
		ErrorType m_errorType;
	};

	class CLogoParserStateException : public CLogoException
	{
	public:
		CLogoParserStateException(wstring errorString) : CLogoException(errorString) {};
	};
}