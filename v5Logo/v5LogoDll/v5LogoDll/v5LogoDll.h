#pragma once
#include "Parser.h"

/*
	v5Logo: command system inspired by Logo to be controlled by voice

	DONE:
	V1
	- Basic scanner
	- Basic Parser
	- Error system
	- Basic set of commands: FORWARD, BACKWARD, LEFT, RIGHT, GOTO, HOME
	- Logo controller (engine) 
	- Step execution support
	- Command vs state vs vector<commands> ? => internal state for the moment
	- REPEAT .. END command (states)
	- Dictionary / Multi-dialect scanner
	- 3D turtle, quaternions
	- 3D turtle, UP / DOWN commands
	- 3D turtle, ROLLRIGHT / ROLLLEFT commands
	- Uncomplete commands i.e. optional parameters
	- PEN up / down command
	- PEN width command
	- Unicode support
	TODO:
	V2
	- Integrate voice commands i.e. grammar == tokens + control 
	- Integrate Oculus VR
	V3
	- Minus / PLus sign ?
	- Control camera as a LogoAgent
	- Speed & modifiers
	- UNDO command
	- SymbolTable, MAKE (support variables, strings?)
	- Procedures (TO... END) + UNDO (?)
	- Event system
*/

namespace v5Logo
{
	class DllExport CLogoEngine : protected CParser
	{
	public:
		CLogoEngine(void);
		virtual ~CLogoEngine(void);
		virtual void Load(const wstring);
		virtual Token Execute();
		virtual wstring GetErrorMessage() const { return mErrorMessage; };

	#pragma warning(disable:4251)
	protected:
		wstring	mErrorMessage;
	};
}