#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include "StarVector2.h"
using namespace Star;

#pragma  comment(lib, "ddraw.lib")
#pragma  comment(lib, "dxguid.lib")

LPDIRECTDRAW7 lpDD;	//DirectDraw����
LPDIRECTDRAWSURFACE7 lpDDSPrimary;	// DirectDraw��Surface
LPDIRECTDRAWSURFACE7 lpDDBack;
LPDIRECTDRAWCLIPPER lpClipper;		//�ü���
HWND hWnd;	//���ھ��
bool bWindowed = true;	//�Ƿ�Ҫ���ڻ�
wchar_t* szMsg = L"Hello World";
const int ScreenWidth = 800;
const int ScreenHeight = 600;

#define  ARGB(a, r, g, b) ((a<<24) + (r<<16) + (g<<8) + b)

// ��������
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool CheckMessages();
BOOL InitWindow(HINSTANCE hInstance, int nCmdShow);
BOOL InitDDraw(void);
void FreeDDraw(void);
void Render();

//*************************************************************************
//Win32������ں��������г�ʼ����������Ϣѭ��
//*************************************************************************
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//��ʼ��Windows����
	if (!InitWindow(hInstance, nShowCmd))
		return FALSE;

	//��ʼ��DirectDraw
	if (!InitDDraw())
	{
		MessageBox(GetActiveWindow(), TEXT("Init DirectDraw Failed"), TEXT("Error"), MB_OK);
		FreeDDraw();
		DestroyWindow(GetActiveWindow());
		return FALSE;
	}

	// ��Ϣѭ��
	while (CheckMessages())
	{
		// for render update
		Render();

		Sleep(1);
	}

	FreeDDraw();
	return 0;
}


//************************************************************************
// ����windows��Ϣ
//************************************************************************
bool CheckMessages()
{
	while (true)
	{
		MSG msg;
		if (!PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			break;
// 		if (!GetMessage(&msg, 0, 0, 0))
// 		{
// 			break;
// 		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);

		// ֻ�йر�����Ϣѭ����Ӧ�ó���Ľ��̲������˳�
		// WM_QUIT�ǲ��ᵽ�ﴰ�ڹ��̵�
		if (msg.message == WM_QUIT)
			return false;
	}

	return true;
}



// ��������
static BOOL InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc; //������

	// ��䴰����ṹ
	wc.style = 0;
	wc.lpfnWndProc = WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//(HBRUSH)GetStockObject(RGB(255, 255, 255));
	wc.lpszMenuName = TEXT("");
	wc.lpszClassName = TEXT("RenderWindow");

	UINT32 iWindowStyle = WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU;

	RECT rc;
	SetRect(&rc, 0, 0, ScreenWidth, ScreenHeight);
	AdjustWindowRect(&rc, iWindowStyle, FALSE);

	// ע�ᴰ����
	RegisterClass(&wc);

	// ��������
	hWnd = CreateWindowEx(0, TEXT("RenderWindow"), TEXT("DirectDraw Demo"), iWindowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
		(rc.right - rc.left),	
		(rc.bottom - rc.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
		return FALSE;

	//��ʾ�����´���
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//*************************************************************************
//��������Ϣ
//*************************************************************************
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//win32Ӧ�ó���������˳����̣�����������ϽǵĹرհ�ť������WM_CLOSE��Ϣ��
	//����Ϣ�����е���DestroyWindow����������WM_DESTROY��Ϣ������Ϣ�����е���
	//PostQuitMessage��0������������WM_QUIT��Ϣ����Ϣ�����С�
	switch (message)
	{
	case WM_KEYDOWN:	//������Ϣ
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:	// ����������Ϣ
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//*************************************************************************
//��ʼ��DirectDraw����
//*************************************************************************
BOOL InitDDraw()
{
	DDSURFACEDESC2 ddsd;	//ҳ������

	// ����DirectDraw����
	if (DirectDrawCreateEx(NULL, (void**)&lpDD, IID_IDirectDraw7, 0) != DD_OK)
		return FALSE;

	if (bWindowed)
	{
		// ����Э���㼶
		if (lpDD->SetCooperativeLevel(GetActiveWindow(), DDSCL_NORMAL) != DD_OK)
			return FALSE;
	}
	else
	{
		// ȡ�ö�ռ��ȫ��ģʽ
		if (lpDD->SetCooperativeLevel(GetActiveWindow(), DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) != DD_OK)
			return FALSE;
	}


	// ������ʾģʽ,�ڴ���ģʽ�²���������ʾģʽ
// 	HRESULT hr;
// 	hr = lpDD->SetDisplayMode(640, 480, 8);
// 	if (hr != DD_OK)
// 		return FALSE;

	ZeroMemory(&ddsd, sizeof(ddsd));

	// �����ҳ����Ϣ
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	// ������ҳ�����
	if (lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL) != DD_OK)
		return FALSE;

	if (bWindowed)
	{
		//�ڴ���ģʽ������Ҫһ����ȫ��ͬ�ġ�����ϵͳ��,�㲻���ڴ���������ʱ����һ���󻺳���Ȼ�����flip()��������ת,
		//��Ϊ���ڴ���ģʽ�²��ܶ����Կ�,����ת�ǽ����������һ�����������ĺ󻺳����ĵ�ַ�Ĺ���,��Ȼ,�㲻���ڴ���ģ
		//ʽ����ô��,��Ϊ��ʱ�������Ӧ�ó�����������
		ddsd.dwFlags = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwWidth = ScreenWidth;
		ddsd.dwHeight = ScreenHeight;

		if (lpDD->CreateSurface(&ddsd, &lpDDBack, NULL) != DD_OK)
		{
			return FALSE;
		}

		//��DirectDraw��������������ʾ������Ļ.Ϊ�˷�ֹ���ڴ���ģʽ����������Ļ����ͼ,
		//����Ը�����������һ���ü���,������������������
		//�����ü���
		if (lpDD->CreateClipper(0, &lpClipper, NULL) != DD_OK)
		{
			lpDDSPrimary->Release();
			lpDD->Release();
			return FALSE;
		}

		if (lpClipper->SetHWnd(0, hWnd) != DD_OK)
		{
			lpClipper->Release();
			lpDDSPrimary->Release();
			lpDD->Release();
			return FALSE;
		}

		if (lpDDSPrimary->SetClipper(lpClipper) != DD_OK)
		{
			lpClipper->Release();
			lpDDSPrimary->Release();
			lpDD->Release();
			return FALSE;
		}

	}

//	HDC hdc;	//�豸�������
// 	if (lpDDSPrimary->GetDC(&hdc) != DD_OK)
// 		return FALSE;
// 
// 	SetBkColor(hdc, RGB(0, 0, 255));
// 	SetTextColor(hdc, RGB(255, 255, 0));
// 	TextOut(hdc, 220, 220, szMsg, lstrlen(szMsg));
// 	lpDDSPrimary->ReleaseDC(hdc);

	return TRUE;
}

WORD & WordAt(int x, int y, BYTE*& pSurface, int pitch)
{
	WORD* pPixel = (WORD*)(pSurface + pitch * y);

	return pPixel[x];
}

DWORD & DWordAt(int x, int y, BYTE*& pSurface, int pitch)
{
	DWORD* pPixel = (DWORD*)(pSurface + pitch * y);

	return pPixel[x];
}

void SetPixel(int x, int y, DWORD color, BYTE*& pSurface, int pitch)
{
	DWordAt(x, y, pSurface, pitch) = color;
}

void RasterizeTriangle(Vector2* pPos0, Vector2* pPos1, Vector2* pPos2)
{
	// sort Position by y
	const Vector2* pVertices[3] = { pPos0, pPos1, pPos2 };



}

// ʵʱ��Ⱦ
void Render()
{
	DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	if (lpDDBack->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
		return;

	BYTE* pSurface = (BYTE*)ddsd.lpSurface;

	// draw
	// ȡ�õ�ǰ��ʾRGBλ����Ŀǰֻ����32λ
	int bpp = ddsd.ddpfPixelFormat.dwRGBBitCount;
	int pitch = ddsd.lPitch;

	for (int i = 0; i < ScreenWidth; i++)
	{
		SetPixel(i, 100, ARGB(0, 125, 0, 255), pSurface, pitch);
	}

	lpDDBack->Unlock(NULL);

	// ����Դ��Ŀ��ľ�������
	POINT pntTopLeft = { 0, 0 };
	ClientToScreen(hWnd, &pntTopLeft);

	RECT rctDestination;
	GetClientRect(hWnd, &rctDestination);
	OffsetRect(&rctDestination, pntTopLeft.x, pntTopLeft.y);

	RECT rctSource;
	SetRect(&rctSource, 0, 0, ScreenWidth, ScreenHeight);
	
	// ���󻺳�ָ�������ݸ��Ƶ�ǰ���ָ������ȥ
	if (lpDDSPrimary->Blt(&rctDestination, lpDDBack, &rctSource, DDBLT_WAIT, 0) != DD_OK)
	{
		return;
	}
}

//*************************************************************************
//�ͷ�DirectDraw����
//*************************************************************************
void FreeDDraw()
{
	if (lpDD != NULL)
	{
		if (lpDDSPrimary != NULL)
		{
			lpDDSPrimary->Release();
			lpDDSPrimary = NULL;
		}
		lpDD->Release();
		lpDD = NULL;
	}
}