#include "StarPresentTarget.h"
#include "StarDevice.h"
#include "StarMath.h"

namespace Star
{
	StarPresentTarget::StarPresentTarget(StarDevice* in_pDevice):
		m_pDevice(in_pDevice)
	{
	}


	StarPresentTarget::~StarPresentTarget()
	{
	}

	StarDevice* StarPresentTarget::GetDevice()
	{
		return m_pDevice;
	}

//-------------------------------------------------------------------------
#ifdef WIN32
	StarPresentTargetWin32::StarPresentTargetWin32(StarDevice* in_pDevice)
		:StarPresentTarget(in_pDevice),
		m_pDirectDraw(0), m_pDirectDrawClipper(0),
		m_pDDSPrimary(0), m_pDDSBack(0),
		m_bDDSurfaceLost(false)
	{
		
	}

	StarPresentTargetWin32::~StarPresentTargetWin32()
	{
		StarDevice_Parameters deviceParam = m_pDevice->GetDeviceParameters();

		if (m_pDDSBack)
		{
			if (deviceParam.bWindowed)
			{
				m_pDDSBack->Release();
			}
			m_pDDSBack = 0;
		}

		SAFE_RELEASE(m_pDDSPrimary);
		SAFE_RELEASE(m_pDirectDrawClipper);

		if (m_pDirectDraw)
		{
			if (deviceParam.bWindowed)
			{

			}
			SAFE_RELEASE(m_pDirectDraw);
		}
	}

	EStarResult StarPresentTargetWin32::Create()
	{
		StarDevice_Parameters deviceParam = m_pDevice->GetDeviceParameters();

		if (!deviceParam.nBackBufferWidth || !deviceParam.nBackBufferHeight)
		{
			return SR_INVALIDPARAM;
		}

		if (FAILED(DirectDrawCreateEx(0, (void**)&m_pDirectDraw, IID_IDirectDraw7, 0)))
		{
			return SR_UKNOWN;
		}

		uint32 nDDFlags = deviceParam.bWindowed ? DDSCL_NORMAL : DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN || DDSCL_ALLOWREBOOT;
		if (FAILED(m_pDirectDraw->SetCooperativeLevel(deviceParam.hDeviceWindow, nDDFlags)))
		{
			return SR_UKNOWN;
		}

		// set display mode
		if (!deviceParam.bWindowed)
		{
			// to do
		}
		else
		{
		}

		if (deviceParam.bWindowed)
		{
			//�ڴ���ģʽ������Ҫһ����ȫ��ͬ�ġ�����ϵͳ��,�㲻���ڴ���������ʱ����һ���󻺳���Ȼ�����flip()��������ת,
			//��Ϊ���ڴ���ģʽ�²��ܶ����Կ�,����ת�ǽ����������һ�����������ĺ󻺳����ĵ�ַ�Ĺ���,��Ȼ,�㲻���ڴ���ģ
			//ʽ����ô��,��Ϊ��ʱ�������Ӧ�ó�����������

			// Create a primary surface
			DDSURFACEDESC2 ddsd;
			ZeroMemory(&ddsd, sizeof(ddsd));
			ddsd.dwSize		= sizeof(ddsd);
			ddsd.dwFlags	= DDSD_CAPS;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

			if (FAILED(m_pDirectDraw->CreateSurface(&ddsd, &m_pDDSPrimary, 0)))
			{
				return SR_UKNOWN;
			}

			// Create a secondary surface -- used as backbuffer
			ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
			ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
			ddsd.dwWidth = deviceParam.nBackBufferWidth;
			ddsd.dwHeight = deviceParam.nBackBufferHeight;

			if (FAILED(m_pDirectDraw->CreateSurface(&ddsd, &m_pDDSBack, 0)))
			{
				return SR_UKNOWN;
			}

			//��DirectDraw��������������ʾ������Ļ.Ϊ�˷�ֹ���ڴ���ģʽ����������Ļ����ͼ,
			//����Ը�����������һ���ü���,������������������
			// Create clipper
			if (FAILED(m_pDirectDraw->CreateClipper(0, &m_pDirectDrawClipper, 0)))
			{
				return SR_UKNOWN;
			}

			// Set Clipper to window
			if (FAILED(m_pDirectDrawClipper->SetHWnd(0, deviceParam.hDeviceWindow)))
			{
				return SR_UKNOWN;
			}

			// attach clipper object to primary surface
			if (FAILED(m_pDDSPrimary->SetClipper(m_pDirectDrawClipper)))
			{
				return SR_UKNOWN;
			}
		}
		else
		{
			// Create a primary surface with one backbuffer if in fullscreen mode
			DDSURFACEDESC2 ddsd;
			ZeroMemory(&ddsd, sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);
			ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
			ddsd.dwBackBufferCount = 1;
			ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
			if (FAILED(m_pDirectDraw->CreateSurface(&ddsd, &m_pDDSPrimary, 0)))
			{
				return SR_UKNOWN;
			}

			// Get DD Backbuffer
			DDSCAPS2 ddCaps = { DDSCAPS_BACKBUFFER };
			if (FAILED(m_pDDSPrimary->GetAttachedSurface(&ddCaps, &m_pDDSBack)))
			{
				return SR_UKNOWN;
			}
		}

		return SR_OK;
	}

	EStarResult StarPresentTargetWin32::Present(const float32* in_pSource, uint32 in_nColorFloats)
	{
		StarDevice_Parameters deviceParam = m_pDevice->GetDeviceParameters();

		// Check for lost DirectDraw surfaces
		if (m_bDDSurfaceLost)
		{
			m_pDDSPrimary->Restore();
			if (deviceParam.bWindowed)
				m_pDDSBack->Restore();

			m_bDDSurfaceLost = false;
		}

		// Lock backbuffer-surface
		DDSURFACEDESC2 ddsd;
		ddsd.dwSize = sizeof(ddsd);
		if (FAILED(m_pDDSBack->Lock(0, &ddsd, DDLOCK_WAIT | DDLOCK_NOSYSLOCK | DDLOCK_WRITEONLY, 0)))
		{
			return SR_UKNOWN;
		}

		// Copy pixel to backbuffer surface
		const uint32 nDestColorBytes = ddsd.ddpfPixelFormat.dwRGBBitCount / 8;
		const uint32 nDestRowCacheBytes = ddsd.lPitch - nDestColorBytes * deviceParam.nBackBufferWidth;
		uint8* pDest = (uint8*)ddsd.lpSurface;

		uint32 nHeight = deviceParam.nBackBufferHeight;
		while (nHeight--)
		{
			uint32 nWidth = deviceParam.nBackBufferWidth;
			while (nWidth--)
			{
				pDest[0] = StarMath::Clamp<int32>(ftol(in_pSource[2] * 255.0f), 0, 255); // b
				pDest[1] = StarMath::Clamp<int32>(ftol(in_pSource[1] * 255.0f), 0, 255); // g
				pDest[2] = StarMath::Clamp<int32>(ftol(in_pSource[0] * 255.0f), 0, 255); // r

				in_pSource += in_nColorFloats;
				pDest += nDestColorBytes;
			}
			pDest += nDestRowCacheBytes;
		}

		m_pDDSBack->Unlock(NULL);

		// Present the backbuffer to screen
		if (deviceParam.bWindowed)
		{
			// ����Դ��Ŀ��ľ�������
			POINT pntTopLeft = { 0, 0 };
			ClientToScreen(deviceParam.hDeviceWindow, &pntTopLeft);

			RECT rctDestination;
			GetClientRect(deviceParam.hDeviceWindow, &rctDestination);
			OffsetRect(&rctDestination, pntTopLeft.x, pntTopLeft.y);

			RECT rctSource;
			SetRect(&rctSource, 0, 0, deviceParam.nBackBufferWidth, deviceParam.nBackBufferHeight);

			// ���󻺳�ָ�������ݸ��Ƶ�ǰ���ָ������ȥ
			if (FAILED(m_pDDSPrimary->Blt(&rctDestination, m_pDDSBack, &rctSource, DDBLT_WAIT, 0)))
			{
				m_bDDSurfaceLost = true;
				return SR_UKNOWN;
			}
		}
		else
		{
			if (FAILED(m_pDDSPrimary->Flip(0, DDFLIP_WAIT)))
			{
				m_bDDSurfaceLost = true;
				return SR_UKNOWN;
			}
		}

		return SR_OK;
	}

#endif // WIN32
}


