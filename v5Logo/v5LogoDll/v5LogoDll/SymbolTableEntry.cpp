#include "stdafx.h"
#include "SymbolTableEntry.h"

namespace v5Logo
{
	CSymbolTableEntry::CSymbolTableEntry(void)
	{
	}

	CSymbolTableEntry::~CSymbolTableEntry(void)
	{
	}
}

/*
namespace MhsLogoParser
{
	public class SymbolTableEntry
	{
		public static readonly SymbolTableEntry None = new NoneSymbolEntry();

		private readonly List<SymbolTableAttribute> attributes = new List<SymbolTableAttribute>();

		public SymbolTableEntry(string name)
		{
			Name = name;
		}

		public ReadOnlyCollection<SymbolTableAttribute> Attributes
		{
			get { return attributes.AsReadOnly(); }
		}

		public string Name { get; private set; }

		public void AddAttribute(SymbolTableAttribute symbolTableAttribute)
		{
			attributes.Add(symbolTableAttribute);
		}

		public bool TryLookupRoutineAttribute(ref SymbolTableRoutineAttribute routineAttribute)
		{
			bool result = false;
			foreach (SymbolTableAttribute attribute in attributes)
			{
				if (attribute.Type == SymbolType.ROUTINE)
				{
					routineAttribute = attribute as SymbolTableRoutineAttribute;
					result = true;
				}
			}
			return result;
		}

		#region Nested type: NoneSymbolEntry

		public class NoneSymbolEntry : SymbolTableEntry
		{
			public NoneSymbolEntry()
				: base(string.Empty)
			{
			}
		}

		#endregion
	}
}
*/