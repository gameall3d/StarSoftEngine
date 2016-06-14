#ifndef __STARVECTOR4_H_
#define __STARVECTOR4_H_

#include "StarBase.h"

namespace Star
{
	class StarVector4
	{
	public:
		float32 x, y, z, w;

	public:
		inline StarVector4(){}
		inline StarVector4(const float32 fX, const float32 fY, const float32 fZ, const float32 fW)
			:x(fX), y(fY), z(fZ), w(fW)
		{
		}

		static const StarVector4 ZERO;	
	};
}

#endif // __STARVECTOR4_H_


