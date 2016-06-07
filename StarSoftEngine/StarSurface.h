#ifndef __STARSUFACE_H_
#define __STARSUFACE_H_

#include "StarTypes.h"
#include "StarRectangle.h"
#include "StarPrerequisites.h"

namespace Star
{
	class StarSurface
	{
	public:
		StarSurface(StarDevice* pDevice);
		~StarSurface();
		EStarResult Create(uint32 nWidth, uint32 nHeight, EColorFormat eColorFormat);

		EStarResult LockRect(void** out_ppData, const StarRectangle* in_pRect);
		EStarResult UnlockRect();
		EStarResult Clear(const StarColor& color, const StarRectangle* pRect);

		uint32 GetFormatFloats();
		uint32 GetWidth();
		uint32 GetHeight();

	private:
		class StarDevice*	m_pDevice;

		EColorFormat	m_eColorFormat;
		uint32				m_nFormatFloats;
		uint32				m_nWidth;
		uint32				m_nHeight;

		bool			m_bLocked;
		StarRectangle	m_PartialLockRect;
		float32*			m_pPartialLockData;

		float32*			m_pData;
	};
}



#endif // end of __STARSUFACE_H_