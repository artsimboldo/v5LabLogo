#include "WndLab.h"
#include "WinLab.inl"
// Memory leaks detection
#ifdef _DEBUG
#include <vld.h>
#endif  
// _DEBUG
using namespace utils;

CWndLab::CWndLab(void) :
	hDC(nullptr),
	hRC(nullptr),
	hWnd(nullptr),
	hInstance(nullptr),
	mFullScreen(false),
	mWidth(0),
	mHeight(0),
	mWidth2(0),
	mHeight2(0),
	mLeft(false),
	mRight(false),
	mForward(false),
	mBackward(false),
	mFreeMouse(true)
{
	// Set initial pos of the camera
	glm::vec3 initialPos(0.f, 2.f, 20.f);
	mOgl.GetCamera().mPosition = initialPos;
	mStart = mBegin = GetTickCount();
	CLogoActor* pActor = new CLogoActor();
	pActor->mName = L"Logo Actor 1";
	mEntities.push_back(unique_ptr<CEntity>(pActor));
	mSelectedActor = pActor;
	/*
	CEntity* pE = new CEntity(L"Entity 1");
	initialPos = glm::vec3(-5.f, 0.f, 0.f);
	pE->mPosition = initialPos;
	mEntities.push_back(pE);
	*/
}

CWndLab::~CWndLab(void) {}

bool CWndLab::Create(const LPCWSTR title, const int width, const int height, const int bits)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height
	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = nullptr;									// No Background Required For GL
	wc.lpszMenuName = nullptr;									// We Don't Want A Menu
	wc.lpszClassName = (LPCWSTR)L"OpenGL";					// Set The Class Name
	mWidth = width;
	mHeight = height;
	mWidth2 = mWidth / 2;
	mHeight2 = mHeight / 2;
	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, (LPCWSTR)L"Failed To Register The Window Class.", (LPCWSTR)L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;											// Return FALSE
	}
	if (mFullScreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(nullptr, (LPCWSTR)L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", (LPCWSTR)L"v5Lab", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				mFullScreen = false;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(nullptr, (LPCWSTR)L"Program Will Now Close.", (LPCWSTR)L"ERROR", MB_OK | MB_ICONSTOP);
				return false;									// Return FALSE
			}
		}
	}
	if (mFullScreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;							// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size
	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		(LPCWSTR)L"OpenGL",					// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		nullptr,								// No Parent Window
		nullptr,								// No Menu
		hInstance,							// Instance
		nullptr)))								// Dont Pass Anything To WM_CREATE
	{
		Destroy();
		MessageBox(nullptr, (LPCWSTR)L"Window Creation Error.", (LPCWSTR)L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		Destroy();								// Reset The Display
		MessageBox(nullptr, (LPCWSTR)L"Can't Create A GL Device Context.", (LPCWSTR)L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		Destroy();								// Reset The Display
		MessageBox(nullptr, (LPCWSTR)L"Can't Find A Suitable PixelFormat.", (LPCWSTR)L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}
	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		Destroy();								// Reset The Display
		MessageBox(nullptr, (LPCWSTR)L"Can't Set The PixelFormat.", (LPCWSTR)L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}
	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		Destroy();								// Reset The Display
		MessageBox(nullptr, (LPCWSTR)L"Can't Create A GL Rendering Context.", (LPCWSTR)L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}
	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		Destroy();								// Reset The Display
		MessageBox(nullptr, (LPCWSTR)L"Can't Activate The GL Rendering Context.", (LPCWSTR)L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}
	// OGL setup
	if (!mOgl.Init(hDC))
	{
		Destroy();
		MessageBox(nullptr, (LPCWSTR)L"Initialization Failed.", (LPCWSTR)L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;								// Return FALSE
	}
	return true;									// Success
}

void CWndLab::Show()
{
	if (!mFullScreen)
	{
		RECT dRect, wRect, cRect;
		GetWindowRect(GetDesktopWindow(), &dRect);
		GetWindowRect(hWnd, &wRect);
		GetClientRect(hWnd, &cRect);
		wRect.right += mWidth - cRect.right;
		wRect.bottom += mHeight - cRect.bottom;
		wRect.right -= wRect.left;
		wRect.bottom -= wRect.top;
		wRect.left = dRect.right / 2 - wRect.right / 2;
		wRect.top = dRect.bottom / 2 - wRect.bottom / 2;
		MoveWindow(hWnd, wRect.left, wRect.top, wRect.right, wRect.bottom, FALSE);
	}
	ShowWindow(hWnd, SW_SHOW);		// Show The Window
	SetForegroundWindow(hWnd);		// Slightly Higher Priority
	SetFocus(hWnd);					// Sets Keyboard Focus To The Window
	SetCurPos(mWidth2, mHeight2);
	mOgl.Resize(mWidth, mHeight);	// Set Up Our Perspective GL Screen
}

void CWndLab::MsgLoop()
{
	MSG Msg;
	while (GetMessage(&Msg, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

void CWndLab::Destroy()
{
	if (mFullScreen)									// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(nullptr, nullptr))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(nullptr, (LPCWSTR)L"Release Of DC And RC Failed.", (LPCWSTR)L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(nullptr, (LPCWSTR)L"Release Rendering Context Failed.", (LPCWSTR)L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = nullptr;										// Set RC To NULL
	}
	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(nullptr, (LPCWSTR)L"Release Device Context Failed.", (LPCWSTR)L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = nullptr;										// Set DC To NULL
	}
	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(nullptr, (LPCWSTR)L"Could Not Release hWnd.", (LPCWSTR)L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = nullptr;										// Set hWnd To NULL
	}
	if (!UnregisterClass((LPCWSTR)L"OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(nullptr, (LPCWSTR)L"Could Not Unregister Class.", (LPCWSTR)L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = nullptr;									// Set hInstance To NULL
	}
}

void CWndLab::OnPaint()
{
	static int sFPS = 0;
	DWORD end = GetTickCount();
	float FrameTime = (end - mBegin) * 0.001f;
	mBegin = end;
	if (end - mStart > 1000)
	{
		SetWindowText(hWnd, wFormat(L"v5Lab Logo at %d fps", sFPS).c_str());
		sFPS = 0;
		mStart = end;
	}
	else
	{
		sFPS++;
	}
	ProcessInput();
	ProcessEntities(mEntities);
	mOgl.Render(mEntities);
	mOgl.SetCommandStatus(mSelectedActor->GetErrorMessage());
	mOgl.RenderText(mSelectedActor->mProgram);
	SwapBuffers(hDC);
}

void CWndLab::ProcessEntities(vector<unique_ptr<CEntity>>& entities)
{
	for (const auto &p : entities)
	{
		p->UpdateEntity();
	}
}

// Will be derived class of CInputManager for keyboard
void CWndLab::ProcessInput()
{
	glm::vec3 speed;
	CCamera *pCam = &mOgl.GetCamera();
	if (mForward)
	{
		if (mFreeMouse)
		{
			speed.x = SPEEDFACTOR * float(sin(glm::radians(pCam->mRotation.y)) * cos(glm::radians(pCam->mRotation.x)));
			speed.y = -SPEEDFACTOR * float(sin(glm::radians(pCam->mRotation.x)));
			speed.z = -SPEEDFACTOR * float(cos(glm::radians(pCam->mRotation.y)) * cos(glm::radians(pCam->mRotation.x)));
		}
		else
		{
			speed.x = SPEEDFACTOR * float(sin(glm::radians(pCam->mRotation.y)) * cos(glm::radians(pCam->mRotation.x)));
			speed.z = -SPEEDFACTOR * float(cos(glm::radians(pCam->mRotation.y)) * cos(glm::radians(pCam->mRotation.x)));
		}
	}
	else if (mBackward)
	{
		if (mFreeMouse)
		{
			speed.x = -SPEEDFACTOR * float(sin(glm::radians(pCam->mRotation.y)) * cos(glm::radians(pCam->mRotation.x)));
			speed.y = SPEEDFACTOR * float(sin(glm::radians(pCam->mRotation.x)));
			speed.z = SPEEDFACTOR * float(cos(glm::radians(pCam->mRotation.y)) * cos(glm::radians(pCam->mRotation.x)));
		}
		else
		{
			speed.x = -SPEEDFACTOR * float(sin(glm::radians(pCam->mRotation.y)) * cos(glm::radians(pCam->mRotation.x)));
			speed.z = SPEEDFACTOR * float(cos(glm::radians(pCam->mRotation.y)) * cos(glm::radians(pCam->mRotation.x)));
		}
	}
	if (mLeft)
	{
		float yRotRad = float(glm::radians(pCam->mRotation.y));
		speed.x += -SPEEDFACTOR * float(cos(yRotRad));
		speed.z += -SPEEDFACTOR * float(sin(yRotRad));
	}
	else if (mRight)
	{
		float yRotRad = float(glm::radians(pCam->mRotation.y));
		speed.x += SPEEDFACTOR * float(cos(yRotRad));
		speed.z += SPEEDFACTOR * float(sin(yRotRad));
	}
	glm::clamp(speed.x, -SPEEDFACTOR, SPEEDFACTOR);
	glm::clamp(speed.y, -SPEEDFACTOR, SPEEDFACTOR);
	glm::clamp(speed.z, -SPEEDFACTOR, SPEEDFACTOR);
	pCam->SetSpeed(speed);
	pCam->Move();
}

void CWndLab::OnSize(int sx, int sy)
{
	if (!mFullScreen)
	{
		mWidth = sx;
		mHeight = sy;
		mWidth2 = mWidth / 2;
		mHeight2 = mHeight / 2;
	}
	mOgl.Resize(sx, sy);
}

void CWndLab::OnKeyDown(int nChar)
{
	switch (nChar)
	{
	case VK_ESCAPE:
		PostQuitMessage(0);
		break;
	case VK_F1:
		Destroy();
		mFullScreen = !mFullScreen;
		Create((LPCWSTR)L"V5Lab Logo", mWidth, mHeight, 16);
		Show();
		break;
	case VK_F9:
		//			mFreeMouse = !mFreeMouse;
		break;
	case VK_RIGHT:
		mRight = true;
		break;
	case VK_LEFT:
		mLeft = true;
		break;
	case VK_UP:
		mForward = true;
		break;
	case VK_DOWN:
		mBackward = true;
		break;
	case VK_RETURN:
		mSelectedActor->Load(mSelectedActor->mProgram.back());
		mSelectedActor->mProgram.push_back(L"");
		break;
	case VK_BACK:
	{
		wstring *pStr = &mSelectedActor->mProgram.back();
		if (pStr->size() > 0)
		{
			pStr->pop_back();
		}
	}
	break;
	case VK_TAB:
		break;
	default:
		mSelectedActor->mProgram.back().push_back(MapVirtualKeyW(nChar, MAPVK_VK_TO_CHAR));
		break;
	}
}

void CWndLab::OnKeyUp(int nChar)
{
	switch (nChar)
	{
	case VK_RIGHT:
		mRight = false;
		break;
	case VK_LEFT:
		mLeft = false;
		break;
	case VK_UP:
		mForward = false;
		break;
	case VK_DOWN:
		mBackward = false;
		break;
	}
}

void CWndLab::OnMouseMove(int cx, int cy)
{
	// filter WM_MOUSEMOVE events (i.e. SetCursorPos)
	if (cx == mWidth2 && cy == mHeight2)
	{
		return;
	}
	glm::vec3 *pCamRot = &mOgl.GetCamera().mRotation;
	pCamRot->x += (cy - mHeight2) / VERTMOUSESENSITIVITY;
	pCamRot->y += (cx - mWidth2) / HORIMOUSESENSITIVITY;
	if (pCamRot->x < -90.0f)
	{
		pCamRot->x = -90.0f;
	}
	else if (pCamRot->x > 90.0f)
	{
		pCamRot->x = 90.0f;
	}
	if (pCamRot->y < -180.0f)
	{
		pCamRot->y += 360.0f;
	}
	else if (pCamRot->y > 180.0f)
	{
		pCamRot->y -= 360.0f;
	}
	// Reset the mouse position to the centre of the window each frame
	SetCurPos(mWidth2, mHeight2);
}

void CWndLab::SetCurPos(int cx, int cy)
{
	POINT Point;
	Point.x = cx;
	Point.y = cy;
	ClientToScreen(hWnd, &Point);
	SetCursorPos(Point.x, Point.y);
}

/*
*
*/

CWndLab Wnd;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		Wnd.OnKeyDown(LOWORD(wParam));
		break;
	case WM_KEYUP:
		Wnd.OnKeyUp(LOWORD(wParam));
		break;
	case WM_KILLFOCUS:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		Wnd.OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
	case 0x020A: // WM_MOUSWHEEL
		break;
	case WM_PAINT:
		Wnd.OnPaint();
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_SETFOCUS:
		break;
	case WM_SIZE:
		Wnd.OnSize(LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int iShow)
{
	if (Wnd.Create((LPCWSTR)L"v5Lab Logo", 1000, 700, 16))
	{
		Wnd.Show();
		Wnd.MsgLoop();
	}
	Wnd.Destroy();
	return 0;
}