#include "stdafx.h"
#include "SymbolTable.h"

namespace v5Logo
{
	CSymbolTable::CSymbolTable(void)
	{
	}

	CSymbolTable::~CSymbolTable(void)
	{
	}
}


/*
namespace MhsLogoParser
{
	public class SymbolTable
	{
		private readonly Dictionary<string, SymbolTableEntry> entries = new Dictionary<string, SymbolTableEntry>();

		public ReadOnlyCollection<SymbolTableEntry> Entries
		{
			get { return new ReadOnlyCollection<SymbolTableEntry>(entries.Values.ToList()); }
		}

		public int NumberOfEntries
		{
			get { return Entries.Count; }
		}

		public SymbolTableEntry Enter(string name)
		{
			SymbolTableEntry entry;
			if (!(entries.TryGetValue(name, out entry)))
			{
				entry = new SymbolTableEntry(name);
				entries.Add(name, entry);
			}
			return entry;
		}

		public SymbolTableEntry Lookup(string name)
		{
			SymbolTableEntry entry;
			if (!entries.TryGetValue(name, out entry))
			{
				return SymbolTableEntry.None;
			}
			return entry;
		}

		public IEnumerable<string> LookupRoutines()
		{
			var result = new List<string>();
			foreach (SymbolTableEntry entry in Entries)
			{
				if (entry.Attributes.Any(attribute => attribute.Type == SymbolType.ROUTINE))
				{
					result.Add(entry.Name);
				}
			}
			return result;
		}
	}
}
*/