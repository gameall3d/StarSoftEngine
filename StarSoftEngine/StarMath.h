#ifndef __STARMATH_H_
#define __STARMATH_H_
//#include "StarBase.h"
#include <assert.h>
#include <algorithm>

namespace Star
{
	class StarMath
	{
	public:
		template<typename T>
		static T Clamp(T val, T minVal, T maxVal)
		{
			assert(minVal <= maxVal && "Invalid clamp range");
			return max(min(val, maxVal), minVal);
		}
	};
}

#endif // end of __STARMATH_H_