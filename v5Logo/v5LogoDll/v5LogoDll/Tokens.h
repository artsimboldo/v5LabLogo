#pragma once

namespace v5Logo
{
	enum class Token
	{
		/* Reserved token */
		REPEAT = 0,
		FORWARD,
		BACK,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		ROLLRIGHT,
		ROLLLEFT,
		HOME,
		MOVETO,
		END,
		CLEAR,
		PENUP,
		PENDOWN,
		PENWIDTH,
		END_OF_RESERVED,
		/* System tokens */
		NUMBER,
		COMMA,
		IDENTIFIER,
		END_OF_FILE,
		NONE,
		EXECUTE,
		PARSE_ERROR,
		END_OF_ENUM
	};
}