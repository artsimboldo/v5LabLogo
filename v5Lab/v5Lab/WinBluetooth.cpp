#include "WinBluetooth.h"
#include "WinLab.inl"
#include <WinSock2.h>
#include <ws2bth.h>
#include <vector>

using namespace utils;

namespace winBluetooth
{
	typedef ULONGLONG bt_addr, *pbt_addr, BT_ADDR, *PBT_ADDR;

	union {
		CHAR buf[5000];
		SOCKADDR_BTH _Unused_;
	} sButuh;

	/*
	struct BtDevice {
	wstring name;
	BT_ADDR btAddr;
	};
	vector<BtDevice*> Devices;
	int SelectedDevice = -1;
	*/

	wstring ErrorMessage;
	SOCKADDR_BTH sockAddr;
	SOCKET btSocket;
	sockaddr* sockAddrClient;

	bool StartAsync(HWND& hWnd)
	{
		WSADATA wsaData;
		DWORD dwResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (dwResult != 0)
		{
			ErrorMessage = wFormat(L"Bluetooth error: cannot startup Winsock, error code %d\n", dwResult);
			return false;
		}

		btSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);

		if(btSocket == INVALID_SOCKET)
		{
			ErrorMessage = L"Critical error! Socket creation failed";
			return false;
		}

		memset (&sockAddr, 0, sizeof(sockAddr));
		sockAddr.addressFamily = AF_BTH;
		sockAddr.serviceClassId = RFCOMM_PROTOCOL_UUID;
		sockAddr.port = BT_PORT_ANY;
		sockAddr.btAddr = 0;
		sockAddrClient = (sockaddr*)&sockAddr;

		if(bind(btSocket,(struct sockaddr *)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			ErrorMessage = L"Error: unable to bind socket";
			return false;
		}

		int result = WSAAsyncSelect(btSocket,
			hWnd,
			WM_SOCKET,
			(FD_CLOSE|FD_ACCEPT|FD_READ));

		if (result)
		{
			ErrorMessage= L"Critical error: WSAAsyncSelect failed";
			return false;
		}

		if (listen(btSocket, 1) == SOCKET_ERROR)
		{
			ErrorMessage = L"Error: Unable to listen!";
			return false;
		}

		WSAQUERYSET service;
		memset(&service, 0, sizeof(service));
		service.dwSize = sizeof(service);
		service.lpszServiceInstanceName = L"gospok";
		service.lpszComment = L"gospok server instance";

		GUID serviceID = OBEXFileTransferServiceClass_UUID;

		service.lpServiceClassId = &serviceID;
		service.dwNumberOfCsAddrs = 1;
		service.dwNameSpace = NS_BTH;

		CSADDR_INFO csAddr;
		memset(&csAddr, 0, sizeof(csAddr));
		csAddr.LocalAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
		csAddr.LocalAddr.lpSockaddr = sockAddrClient;
		csAddr.iSocketType = SOCK_STREAM;
		csAddr.iProtocol = BTHPROTO_RFCOMM;
		service.lpcsaBuffer = &csAddr;

		if (0 != WSASetService(&service, RNRSERVICE_REGISTER, 0))
		{
			ErrorMessage = L"Service registration failed....";
//			printf("%d\n", GetLastErrorMessage(GetLastError()));
			return false;
		}

		return true;
	}

	//29913A2D-EB93-40cf-BBB8-DEEE26452197
	bool Accept(WPARAM wParam)
	{
		int size = sizeof(sockaddr);
		btSocket = accept(wParam, sockAddrClient, &size);
		if (btSocket == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			ErrorMessage = wFormat(L"Accept: error %d", error);
			return false;
		}
		return true;
	}

	bool Stop()
	{
		shutdown(btSocket,SD_BOTH);
		closesocket(btSocket);
		//		DeleteVector(&Devices);
		if(WSACleanup() != 0)
		{
			ErrorMessage = wFormat(L"Bluetooth error: WSACleanup() failed with error code %ld\n", WSAGetLastError());
			return false;
		}
		return true;
	}

	/*
	bool FindDevices()
	{
	WSAQUERYSET wsaq;
	HANDLE hLookup;
	DWORD dwSize  = sizeof(sButuh.buf);
	ZeroMemory(&wsaq, sizeof(wsaq));
	wsaq.dwSize = sizeof(wsaq);
	wsaq.dwNameSpace = NS_BTH; // namespace MUST be NS_BTH for bluetooth queries
	wsaq.lpcsaBuffer = NULL;

	// initialize searching procedure
	if (WSALookupServiceBegin(&wsaq, LUP_CONTAINERS, &hLookup) == SOCKET_ERROR)
	{
	ErrorMessage = wFormat(L"Bluetooth error: WSALookupServiceBegin() failed %d\r\n", WSAGetLastError());
	return false;
	}

	//	BT_ADDR btAddr;
	//	BOOL bHaveName;
	//	DWORD dwNameSpace;
	LPWSAQUERYSET pwsaResults = (LPWSAQUERYSET) sButuh.buf;
	ZeroMemory(pwsaResults, sizeof(WSAQUERYSET));
	pwsaResults->dwSize = sizeof(WSAQUERYSET);
	pwsaResults->dwNameSpace = NS_BTH;
	pwsaResults->lpBlob = NULL;

	// iterate through all found devices, returning name and address
	DeleteVector(&Devices);

	while (WSALookupServiceNext(hLookup, LUP_RETURN_NAME | LUP_RETURN_ADDR, &dwSize, pwsaResults) == 0)
	{
	BtDevice* pDevice = new BtDevice();
	pDevice->name = pwsaResults->lpszServiceInstanceName;
	pDevice->btAddr = ((SOCKADDR_BTH *)pwsaResults->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr;
	Devices.push_back(pDevice);
	// Verify the returned name
	//bHaveName = (pwsaResults->lpszServiceInstanceName) && *(pwsaResults->lpszServiceInstanceName);
	//dwNameSpace = pwsaResults->dwNameSpace;
	// print name and address
	//		wprintf (L"Name\tNAP Address\tSAP Address\tName Space\n");
	//		wprintf (L"%s\t0X%04X\t\t0X%08X\t0X%0X\n", pwsaResults->lpszServiceInstanceName, GET_NAP(btAddr), GET_SAP(btAddr),
	//			dwNameSpace);
	}

	// Terminate iteration
	if(WSALookupServiceEnd(hLookup) != 0)
	{
	ErrorMessage = wFormat(L"Bluetooth error: WSALookupServiceEnd(hLookup) failed with error code %ld\n", WSAGetLastError());
	return false;
	}

	// TODO: Service discovery

	SelectedDevice = Devices.empty() ? -1 : 0;
	return true;
	}

	bool SelectNextDevice() 
	{
	if (SelectedDevice >= 0)
	{
	SelectedDevice++;
	if (SelectedDevice == Devices.size())
	{
	SelectedDevice = 0;
	}
	return true;
	}
	return false;
	}

	wstring GetSelectedDeviceName()
	{ 
	return SelectedDevice < 0 ? L"No device available!" : Devices[SelectedDevice]->name; 
	}
	*/

	wstring GetErrorMessage()
	{ 
		return ErrorMessage; 
	}
}