#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include "StarVector2.h"
#include "StarBase.h"
#include "StarPrerequisites.h"
#include "StarDevice.h"
#include "StarRenderTarget.h"
#include "StarSurface.h"
#include "StarTypes.h"
#include "Star3D.h"
#include "StarColor.h"
#include "StarTexture.h"

using namespace Star;

#pragma  comment(lib, "ddraw.lib")
#pragma  comment(lib, "dxguid.lib")

HWND hWnd;	//窗口句柄
wchar_t* szMsg = L"Hello World";
const int ScreenWidth = 800;
const int ScreenHeight = 600;

BYTE* pSurface = NULL;
int pitch = 0;
StarVector2* Pos0;
StarVector2* Pos1;
StarVector2* Pos2;

StarVertexData* WorldPos0;
StarVertexData* WorldPos1;
StarVertexData* WorldPos2;

StarVertexData mesh[8] =
{
	{ {  1, -1,  1, 1 }, { 1.0f, 0.2f, 0.2f }, { 0, 0 } },
	{ { -1, -1,  1, 1 }, { 0.2f, 1.0f, 0.2f }, { 0, 1 } },
	{ { -1,  1,  1, 1 }, { 0.2f, 0.2f, 1.0f }, { 1, 1 } },
	{ {  1,  1,  1, 1 }, { 1.0f, 0.2f, 1.0f }, { 1, 0 } },
	{ {  1, -1, -1, 1 }, { 1.0f, 1.0f, 0.2f }, { 0, 0 } },
	{ { -1, -1, -1, 1 }, { 0.2f, 1.0f, 1.0f }, { 0, 1 } },
	{ { -1,  1, -1, 1 }, { 1.0f, 0.3f, 0.3f }, { 1, 1 } },
	{ {  1,  1, -1, 1 }, { 0.2f, 1.0f, 0.3f }, { 1, 0 } },
};

// Test Device
Star3D*		pStar3D;
StarDevice* pDevice;
StarSurface* pColorBuffer;
StarSurface* pDepthBuffer;
StarRenderTarget* pRenderTarget;


// 函数声明
LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool CheckMessages();
BOOL InitWindow(HINSTANCE hInstance, int nCmdShow);
void InitData();
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
// 	if (!InitDDraw())
// 	{
// 		MessageBox(GetActiveWindow(), TEXT("Init DirectDraw Failed"), TEXT("Error"), MB_OK);
// 		FreeDDraw();
// 		DestroyWindow(GetActiveWindow());
// 		return FALSE;
// 	}

	InitData();

	// 消息循环
	while (CheckMessages())
	{
		// for render update
		Render();

		Sleep(1);
	}

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
	hWnd = CreateWindowEx(0, TEXT("RenderWindow"), TEXT("StarSoftRender Demo"), iWindowStyle, CW_USEDEFAULT, CW_USEDEFAULT,
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

void InitData()
{
	Pos0 = new StarVector2(100, 100);
	Pos1 = new StarVector2(100, 200);
	Pos2 = new StarVector2(300, 300);

	WorldPos0 = new StarVertexData();
	WorldPos1 = new StarVertexData();
	WorldPos2 = new StarVertexData();
	WorldPos0->pos = StarVector4(1, -1, 1, 1);
	WorldPos1->pos = StarVector4(-1, -1, 1, 1);
	WorldPos2->pos = StarVector4(-1, 1, 1, 1);

	WorldPos0->color = StarColor::Red;
	WorldPos1->color = StarColor::Green;
	WorldPos2->color = StarColor::Blue;

	StarDevice_Parameters deviceParam;
	deviceParam.hDeviceWindow = hWnd;
	deviceParam.bWindowed = true;
	deviceParam.nBackBufferHeight = ScreenHeight;
	deviceParam.nBackBufferWidth = ScreenWidth;

	pStar3D = Star3DCreate(0);
	pStar3D->CreateDevice(&pDevice, &deviceParam);
	
	pRenderTarget = new StarRenderTarget(pDevice);
	pDevice->CreateSurface(&pColorBuffer, ScreenWidth, ScreenHeight, CFMT_R32G32B32A32);
	pDevice->SetRenderTarget(pRenderTarget);
	pRenderTarget->SetColorBuffer(pColorBuffer);

	pDevice->CreateSurface(&pDepthBuffer, ScreenWidth, ScreenHeight, CFMT_R32);
	pRenderTarget->SetDepthBuffer(pDepthBuffer);
	pDepthBuffer->Clear(StarColor(0, 0, 0, 0), NULL);

	// create texture
	StarTexture* pTexture;
	float32* pTextureData;
	pDevice->CreateTexture(&pTexture, 256, 256, CFMT_R32G32B32A32);
	pTexture->LockRect((void**)&pTextureData, NULL);

	// manual init texture data
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			int x = i / 32;
			int y = j / 32;
			StarColor* pCurData = (StarColor*)&pTextureData[4 * (i * 256 + j)];
			*pCurData = ((x + y) & 1) ? StarColor::White : StarColor::Blue;
		}
	}

	pTexture->UnlockRect();

	pDevice->SetTexture(0, pTexture);

	float aspect = (float32)ScreenWidth / ScreenHeight;
	StarVector3 eyePos(2.0f, 2.0f, -5.0f);
	StarVector3 lookatPos(0.0f, 0.0f, 0.0f);
	StarVector3 upDir(0.0f, 1.0f, 0.0f);

	StarMatrix44 matView;
	matView = StarMatrix44::BuildMatrixLookAtLH(eyePos, lookatPos, upDir);
	pDevice->SetTransform(STST_VIEW, &matView);

	StarMatrix44 matProj;
	matProj = StarMatrix44::BuildMatrixPerspectiveFOVLH(STAR_PI / 3, aspect, 1.0f, 100.0f);
	pDevice->SetTransform(STST_PROJECTION, &matProj);
}

void DrawPlane(uint32 a, uint32 b, uint32 c, uint32 d)
{
	StarVertexData p0 = mesh[a], p1 = mesh[b], p2 = mesh[c], p3 = mesh[d];

	p0.UV.x = 0, p0.UV.y = 0, p1.UV.x = 0, p1.UV.y = 1;
	p2.UV.x = 1, p2.UV.y = 1, p3.UV.x = 1, p3.UV.y = 0;

	pDevice->DrawTriangle(&p0, &p1, &p2);
	pDevice->DrawTriangle(&p2, &p3, &p0);
}

void DrawBox()
{
	DrawPlane(0, 1, 2, 3);
	DrawPlane(4, 5, 6, 7);
	DrawPlane(0, 4, 5, 1);
	DrawPlane(1, 5, 6, 2);
	DrawPlane(2, 6, 7, 3);
	DrawPlane(3, 7, 4, 0);
}

// 实时渲染
void Render()
{
	pDevice->PreRender();
	//pDevice->RasterizeTriangle(Pos0, Pos1, Pos2);
	//pDevice->DrawTriangle(WorldPos0, WorldPos1, WorldPos2);
	DrawBox();
	//DrawPlane(0, 1, 2, 3);
	pDevice->PostRender();

	pDevice->Present(pRenderTarget);
}
