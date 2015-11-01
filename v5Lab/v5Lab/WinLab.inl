#pragma once
#include <vector>
#include <string>
#include <stdarg.h>

namespace utils
{
	inline std::wstring wFormat(const wchar_t* fmt, ...)
	{
		std::wstring str;
		va_list vl;
		va_start(vl, fmt);
		int size = _vscwprintf(fmt, vl);
		str.resize(++size);
		_vsnwprintf_s((wchar_t*)str.data(), size, _TRUNCATE, fmt, vl);
		va_end(vl);
		return str;
	}
}