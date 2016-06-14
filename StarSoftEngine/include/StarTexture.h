#ifndef __STARTEXTURE_H_
#define __STARTEXTURE_H_

#include "StarPrerequisites.h"
#include "StarTypes.h"

namespace Star
{
	class StarTexture
	{
	public:
		StarTexture(StarDevice* pDevice);
		~StarTexture();

		EStarResult Create(uint32 nWidth, uint32 nHeight, EColorFormat eColorFormat);
		EStarResult LockRect(void** out_ppData, const StarRectangle* pRect);
		EStarResult UnlockRect();

		EStarResult SampleTexture(StarColor& out_Color, float32 fU, float32 fV);

	private:
		StarSurface*	m_pSurfaceData;
		StarDevice*		m_pDevice;
	};
}


#endif // __STARTEXTURE_H_