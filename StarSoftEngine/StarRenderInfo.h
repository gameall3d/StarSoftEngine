#ifndef __STARRENDERINFO_H_
#define __STARRENDERINFO_H_

#include "StarBase.h"
#include "StarPrerequisites.h"

namespace Star
{
	class StarRenderInfo
	{
	public:
		StarRenderInfo();
		~StarRenderInfo();

	public:
		float32*	m_pFrameData;
		uint32		m_nColorFloats;
		uint32		m_nColorBufferPitch;

		void (StarDevice::*fpRasterizeScanline)(uint32, uint32, uint32);
		void (StarDevice::*fpDrawPixel)(uint32, uint32);
	};

}


#endif // __STARRENDERINFO_H_


