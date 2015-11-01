#pragma once

#include <map>
#include "SymbolTableEntry.h"

using namespace std;

namespace v5Logo
{
	typedef map<string, CSymbolTableEntry*> TSymbolTable;

	class CSymbolTable
	{
	public:
		CSymbolTable(void);
		~CSymbolTable(void);

	private:
		TSymbolTable m_SymbolTable;

//		private readonly Dictionary<string, SymbolTableEntry> entries = new Dictionary<string, SymbolTableEntry>();

	};
}