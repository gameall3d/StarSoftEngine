#ifndef __STARRENDERTARGET_H_
#define __STARRENDERTARGET_H_

#include "StarBase.h"
#include "StarPrerequisites.h"

namespace Star
{
	class StarRenderTarget
	{
	public:
		StarRenderTarget(StarDevice* pDevice);
		~StarRenderTarget();

		EStarResult SetColorBuffer(StarSurface* in_pColorBuffer);
		StarSurface* GetColorBuffer();

		EStarResult SetDepthBuffer(StarSurface* in_pDepthBuffer);
		StarSurface* GetDepthBuffer();

	private:
		class StarDevice*		m_pDevice;
		class StarSurface*		m_pColorBuffer;
		class StarSurface*		m_pDepthBuffer;
	};

}


#endif // __STARRENDERTARGET_H_

