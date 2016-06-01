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

	private:
		class StarDevice*		m_pDevice;
		class StarSurface*		m_pColorBuffer;
	};

}


#endif // __STARRENDERTARGET_H_

