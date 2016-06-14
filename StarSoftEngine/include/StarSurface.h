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

		void SamplePoint(StarColor& out_Color, float32 fU, float32 fV);

		uint32 GetFormatFloats();
		uint32 GetWidth();
		uint32 GetHeight();

	private:
		class StarDevice*	m_pDevice;

		EColorFormat	m_eColorFormat;
		uint32				m_nFormatFloats;
		uint32				m_nWidth;
		uint32				m_nHeight;
		uint32				m_nWidthMin1;	// m_nWidth - 1, for texture sample
		uint32				m_nHeightMin1;  // m_nHeight - 1, for texture sample

		bool			m_bLocked;
		StarRectangle	m_PartialLockRect;
		float32*			m_pPartialLockData;

		float32*			m_pData;
	};
}



#endif // end of __STARSUFACE_H_