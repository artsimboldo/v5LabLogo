// v5LogoDllConsole.cpp : définit le point d'entrée pour l'application console.
//

// Memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG


#include "stdafx.h"
#include <iostream>
#include <string>
#include <exception>
#include "v5LogoDll.h"

using namespace std;
using namespace v5Logo;

static void TestScanner()
{
	CScanner* pScanner =  new CScanner("REPEAT 4 [FORWARD 10 RIGHT 90]");

	Token tok;
	do
	{
		try 
		{
			tok = pScanner->Scan();
			cout << pScanner->GetTokenString(tok) << endl;
		}
		catch(exception ex)
		{
			cout << ex.what() << endl;
			break;
		}
	} while(tok != Token::END_OF_FILE);

	delete pScanner;
}

static void TestParser(string program)
{
	CSymbolTable* pSymbolTable = new CSymbolTable();
	CScanner* pScanner = new CScanner(program);
	CParser* pParser = new CParser(pScanner, pSymbolTable);

	try
	{
		cout << "Success!" << endl;
		TCommands* pCommands = pParser->ParseLogoProgram();
		TCommands::iterator p;
		for(p = pCommands->begin(); p != pCommands->end(); ++p)
		{
			// TODO: display commands
		}
		pParser->DeleteCommands(pCommands);
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
		
	delete pParser;
	delete pScanner;
	delete pSymbolTable;
}

static void TestExecute()
{
	cout << "\n______________________________________________\n" 
		<< "TEST EXECUTE" 
		<< endl << endl;

	CLogoEngine logo;
	string program("REPEAT 2 [RIGHT 90 FORWARD 10]");
	cout << "Program:\n" << program << "\n\nTrace:\n";
	logo.Execute(program);
}

static void TestInput()
{
	const string program[] = {
		"MOVETO",
		"10",
		",",
		"10",
		"REPEAT",
		"2",
		"[",
		"CLEAR",
		"FORWARD",
		"10",
		"]"
	};

	cout << "\n______________________________________________\n" 
		<< "TEST INPUT" 
		<< endl << endl;

	CLogoEngine logo;
	vector<string> inputs(program, end(program));
	vector<string>::iterator p;
	for(p = inputs.begin(); p != inputs.end(); ++p)
	{
		logo.Input(*p);
	}
}

static void TestUndo()
{
	const string program[] = {
		"MOVETO 10, 10",
		"RIGHT 90",
		"FORWARD 10",
		"CLEAR",
		"REPEAT 4 [FORWARD 10 LEFT 90]"
	};

	cout << "\n______________________________________________\n" 
		<< "TEST UNDO" 
		<< endl << endl;

	CLogoEngine logo;
	vector<string> inputs(program, end(program));
	vector<string>::iterator p;
	CTurtleState* pState = logo.GetState();
	printf("X = %d, Y = %d, ANGLE = %d \n", pState->m_X, pState->m_Y, pState->m_Angle);
	for(p = inputs.begin(); p != inputs.end(); ++p)
	{
		logo.Input(*p);
		pState = logo.GetState();
		printf("X = %d, Y = %d, ANGLE = %d \n", pState->m_X, pState->m_Y, pState->m_Angle);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Memory leak detection
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

//	TestScanner(); // simple scanner test case
//  TestParser("FORWARD 1"); // success 1 command test case
//	TestParser("MOVETO 10 0"); // catch missing token error test case
//	TestParser("MOVETO -10, 0"); // catch lexical error test case
//	TestParser("REPEAT 10 [CLEAR MOVETO 10,0 RIGHT 45 REPEAT 2 [MOVETO 1,0]]"); // success + recursive delete test case test case

	TestExecute();
	TestInput();
	TestUndo();

	_CrtDumpMemoryLeaks();
	return 0;
}
