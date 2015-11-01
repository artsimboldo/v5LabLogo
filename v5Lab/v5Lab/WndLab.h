#pragma once
#include "OGLRenderer.h"
#include "LogoActor.h"
#include <string>
#include <vector>
#define VERTMOUSESENSITIVITY	10.0f
#define HORIMOUSESENSITIVITY	10.0f
#define SPEEDFACTOR				0.005f
using namespace std;
using namespace v5Logo;

class CWndLab
{
protected:
	HDC					hDC;				// Private GDI Device Context
	HGLRC				hRC;				// Permanent Rendering Context
	HWND				hWnd;				// Holds Our Window Handle
	HINSTANCE			hInstance;			// Holds The Instance Of The Application
	COGLRenderer		mOgl;				// Holds the instance of the ogl renderer
	bool				mFullScreen;		// Fullscreen Flag Set To Fullscreen Mode By Default
	int					mWidth, mHeight;
	int					mWidth2, mHeight2;
	vector<unique_ptr<CEntity>> mEntities;	// Simple unordered list of scene objects
	CLogoActor*			mSelectedActor;		// Selected Actor available for commands

public:
	CWndLab(void);
	~CWndLab(void);
	bool Create(const LPCWSTR, const int, const int, const int);
	void Show();
    void MsgLoop();
    void Destroy();
	void ProcessInput();
	void ProcessEntities(vector<unique_ptr<CEntity>>&);
	void OnPaint();
	void OnSize(int, int);
	void OnKeyDown(int);
	void OnKeyUp(int);
	void OnMouseMove(int, int);
	void SetCurPos(int, int);

private:
	bool mLeft;
	bool mRight;
	bool mForward;
	bool mBackward;
	bool mFreeMouse;
	DWORD mBegin;
	DWORD mStart;
};

/*
*
*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow);