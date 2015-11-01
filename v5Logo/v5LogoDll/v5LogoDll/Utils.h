#pragma once
#include <vector>
#include <algorithm>
#include <functional> 
#include <cctype>
#include <cstdarg>
using namespace std;

namespace v5Logo
{
	inline wstring wFormat(const wchar_t* fmt, ...)
	{
		wstring str;
		va_list vl;
		va_start(vl, fmt);
		int size = _vscwprintf(fmt, vl);
		str.resize(++size);
		_vsnwprintf_s((wchar_t*)str.data(), size, _TRUNCATE, fmt, vl);
		va_end(vl);
		return str;
	}

	/*
	inline string format(const char* fmt, ...)
	{
		string str;
		va_list vl;
		va_start(vl, fmt);
		int size = _vscprintf(fmt, vl);
		str.resize(++size);
		vsnprintf_s((char*)str.data(), size, _TRUNCATE, fmt, vl);
		va_end(vl);
		return str;
	}
	*/

	// make string to uppercase
	static inline void ToUpper(wstring& s)
	{
		transform(s.begin(), s.end(), s.begin(), ::toupper);
	}

	// make string to uppercase
	static inline void ToLower(wstring& s)
	{
		transform(s.begin(), s.end(), s.begin(), ::tolower);
	}

	// trim from start
	static inline wstring& TrimLeft(wstring &s) 
	{
			s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun(iswspace))));
			s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun(iswcntrl))));
			return s;
	}

	// trim from end
	static inline wstring& TrimRight(wstring &s) 
	{
			s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun(iswspace))).base(), s.end());
			s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun(iswcntrl))).base(), s.end());
			return s;
	}

	// trim from both ends
	static inline wstring& Trim(wstring &s) 
	{
		return TrimLeft(TrimRight(s));
	}

}