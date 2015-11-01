#pragma once
#pragma comment(lib, "Ws2_32.lib")
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

using namespace std;

namespace winBluetooth
{
	#define MAX_NAME	248
	#define WM_SOCKET	104

//	bool Open();
//	bool Close();
	bool StartAsync(HWND&);
	bool Stop();
	bool Accept(WPARAM);
//	bool FindDevices();
//	bool SelectNextDevice();
//	wstring GetSelectedDeviceName();
	wstring GetErrorMessage();
}