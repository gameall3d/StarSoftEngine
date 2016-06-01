
#ifndef __Vector2_H_
#define __Vector2_H_

#include "StarBase.h"

namespace Star
{
	class StarVector2
	{
	public:
		float32 x, y;

	public:
		inline StarVector2()
		{

		}

		inline StarVector2(const float32 fX, const float32 fY)
			:x(fX), y(fY)
		{

		}
	};
}


#endif