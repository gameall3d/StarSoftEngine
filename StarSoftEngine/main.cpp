#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include "StarVector2.h"
using namespace Star;

#pragma  comment(lib, "ddraw.lib")
#pragma  comment(lib, "dxguid.lib")

LPDIRECTDRAW7 lpDD;	//DirectDraw对象
LPDIRECTDRAWSURFACE7 lpDDSPrimary;	// DirectDraw主Surface
LPDIRECTDRAWSURFACE7 lpDDBack;
LPDIRECTDRAWCLIPPER lpClipper;		//裁剪器
HWND hWnd;	//窗口句柄
bool bWindowed = true;	//是否要窗口化
wchar_t* szMsg = L"Hello World";
const int ScreenWidth = 800;
const int ScreenHeight = 600;

#define  ARGB(a, r, g, b) ((a<<24) + (r<<16) + (g<<8) + b)

// 函数声明
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool CheckMessages();
BOOL InitWindow(HINSTANCE hInstance, int nCmdShow);
BOOL InitDDraw(void);
void FreeDDraw(void);
void Render();

//*************************************************************************
//Win32程序入口函数，进行初始化工作和消息循环
//*************************************************************************
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	//初始化Windows窗口
	if (!InitWindow(hInstance, nShowCmd))
		return FALSE;

	//初始化DirectDraw
	if (!InitDDraw())
	{
		MessageBox(GetActiveWindow(), TEXT("Init DirectDraw Failed"), TEXT("Error"), MB_OK);
		FreeDDraw();
		DestroyWindow(GetActiveWindow());
		return FALSE;
	}

	// 消息循环
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
// 处理windows消息
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

		// 只有关闭了消息循环，应用程序的进程才真正退出
		// WM_QUIT是不会到达窗口过程的
		if (msg.message == WM_QUIT)
			return false;
	}

	return true;
}



// 创建窗口
static BOOL InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc; //窗口类

	// 填充窗口类结构
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

	// 注册窗口类
	RegisterClass(&wc);

	// 创建窗口
	hWnd = CreateWindowEx(0, TEXT("RenderWindow"), TEXT("DirectDraw Demo"), iWindowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
		(rc.right - rc.left),	
		(rc.bottom - rc.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
		return FALSE;

	//显示并更新窗口
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//*************************************************************************
//处理窗口消息
//*************************************************************************
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//win32应用程序的完整退出过程：点击窗口右上角的关闭按钮，发送WM_CLOSE消息。
	//此消息处理中调用DestroyWindow函数，发送WM_DESTROY消息。此消息处理中调用
	//PostQuitMessage（0）函数，发送WM_QUIT消息到消息队列中。
	switch (message)
	{
	case WM_KEYDOWN:	//按键消息
		switch (wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:	// 窗口销毁消息
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//*************************************************************************
//初始化DirectDraw环境
//*************************************************************************
BOOL InitDDraw()
{
	DDSURFACEDESC2 ddsd;	//页面描述

	// 创建DirectDraw对像
	if (DirectDrawCreateEx(NULL, (void**)&lpDD, IID_IDirectDraw7, 0) != DD_OK)
		return FALSE;

	if (bWindowed)
	{
		// 设置协调层级
		if (lpDD->SetCooperativeLevel(GetActiveWindow(), DDSCL_NORMAL) != DD_OK)
			return FALSE;
	}
	else
	{
		// 取得独占和全屏模式
		if (lpDD->SetCooperativeLevel(GetActiveWindow(), DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) != DD_OK)
			return FALSE;
	}


	// 设置显示模式,在窗口模式下不能设置显示模式
// 	HRESULT hr;
// 	hr = lpDD->SetDisplayMode(640, 480, 8);
// 	if (hr != DD_OK)
// 		return FALSE;

	ZeroMemory(&ddsd, sizeof(ddsd));

	// 填充主页面信息
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	// 创建主页面对象
	if (lpDD->CreateSurface(&ddsd, &lpDDSPrimary, NULL) != DD_OK)
		return FALSE;

	if (bWindowed)
	{
		//在窗口模式下你需要一个完全不同的“缓冲系统”,你不能在创建主表面时连接一个后缓冲区然后调用flip()函数来翻转,
		//因为你在窗口模式下不能独享显卡,而翻转是交换主表面和一个与其相连的后缓冲区的地址的过程,显然,你不能在窗口模
		//式下这么做,因为此时你和其它应用程序共享主表面
		ddsd.dwFlags = DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwWidth = ScreenWidth;
		ddsd.dwHeight = ScreenHeight;

		if (lpDD->CreateSurface(&ddsd, &lpDDBack, NULL) != DD_OK)
		{
			return FALSE;
		}

		//在DirectDraw中主表面往往表示整个屏幕.为了防止你在窗口模式下在整个屏幕里作图,
		//你可以给主表面连接一个裁剪器,并将其与主窗口相连
		//创建裁剪器
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

//	HDC hdc;	//设备环境句柄
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

// 实时渲染
void Render()
{
	DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	if (lpDDBack->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL) != DD_OK)
		return;

	BYTE* pSurface = (BYTE*)ddsd.lpSurface;

	// draw
	// 取得当前显示RGB位数，目前只处理32位
	int bpp = ddsd.ddpfPixelFormat.dwRGBBitCount;
	int pitch = ddsd.lPitch;

	for (int i = 0; i < ScreenWidth; i++)
	{
		SetPixel(i, 100, ARGB(0, 125, 0, 255), pSurface, pitch);
	}

	lpDDBack->Unlock(NULL);

	// 计算源和目标的矩形区域
	POINT pntTopLeft = { 0, 0 };
	ClientToScreen(hWnd, &pntTopLeft);

	RECT rctDestination;
	GetClientRect(hWnd, &rctDestination);
	OffsetRect(&rctDestination, pntTopLeft.x, pntTopLeft.y);

	RECT rctSource;
	SetRect(&rctSource, 0, 0, ScreenWidth, ScreenHeight);
	
	// 将后缓冲指定区内容复制到前面层指定区域去
	if (lpDDSPrimary->Blt(&rctDestination, lpDDBack, &rctSource, DDBLT_WAIT, 0) != DD_OK)
	{
		return;
	}
}

//*************************************************************************
//释放DirectDraw对象
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