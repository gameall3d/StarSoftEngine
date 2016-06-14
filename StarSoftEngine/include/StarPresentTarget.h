#ifndef __STARPRESENTTARGET_H_
#define __STARPRESENTTARGET_H_

#include "StarBase.h"
#include "StarPrerequisites.h"


namespace Star
{
	class StarPresentTarget
	{
	public:
		StarPresentTarget(StarDevice* in_pDevice);
		~StarPresentTarget();

		virtual EStarResult Create() = 0;
		virtual EStarResult Present(const float32* in_pSource, uint32 in_nColorFloats) = 0;
		StarDevice* GetDevice();

	protected:
		StarDevice* m_pDevice;
	};

	// Platform-dependent code

#ifdef WIN32
#include <ddraw.h>
#pragma comment( lib, "ddraw.lib")
#pragma comment( lib, "dxguid.lib")

	class StarPresentTargetWin32 : public StarPresentTarget
	{
	public:
		StarPresentTargetWin32(StarDevice* in_pDevice);
		~StarPresentTargetWin32();

		EStarResult Create();
		EStarResult Present(const float32* in_pSource, uint32 in_nColorFloats);

	private:
		LPDIRECTDRAW7			m_pDirectDraw;
		LPDIRECTDRAWCLIPPER		m_pDirectDrawClipper;
		LPDIRECTDRAWSURFACE7	m_pDDSPrimary;	// DirectDrawÖ÷Surface
		LPDIRECTDRAWSURFACE7	m_pDDSBack;
		bool					m_bDDSurfaceLost;
		
	};



#endif // WIN32

}



#endif // end of __STARPRESENTTARGET_H_
