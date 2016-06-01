#ifndef __STARDEVICE_H_
#define __STARDEVICE_H_

#include "StarBase.h"
#include "StarPrerequisites.h"
#include "StarTypes.h"

namespace Star
{
	class StarDevice
	{
	protected:
		friend class Star3D;
		StarDevice(Star3D* in_pStar3D, const StarDevice_Parameters* in_pDeviceParameters);
		~StarDevice();
		
		EStarResult Create();

	public:
		EStarResult CreateSurface(StarSurface** out_ppSurface, uint32 nWidth, uint32 nHeight, EColorFormat eColorFormat);

		EStarResult PreRender();
		EStarResult Present(StarRenderTarget* in_pRenderTarget);
		void PostRender();

		void RasterizeTriangle(StarVector2* pPos0, StarVector2* pPos1, StarVector2* pPos2);
		void RasterizeScanline(int32 nYPos, int32 nStartXPos, int32 nEndXpos);


		const StarDevice_Parameters& GetDeviceParameters();
		void SetRenderTarget(StarRenderTarget* in_pRenderTarget);
		StarRenderTarget* GetRenderTarget();

	private:
		StarDevice_Parameters m_DeviceParameters;
		StarPresentTarget* m_pPresentTarget;
		StarRenderTarget* m_pRenderTarget;
		StarRenderInfo* m_pRenderInfo;
	};
}


#endif // end of __SARTDEVICE_H_

