#ifndef __STARMATH_H_
#define __STARMATH_H_
//#include "StarBase.h"
#include <assert.h>
#include <algorithm>
#include "StarPrerequisites.h"


#define  STAR_PI 3.141592654f

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

		static inline float32 Sqrt(float32 fValue) { return sqrt(fValue);  }
	};
}

#include "StarVector2.h"
#include "StarVector3.h"
#include "StarVector4.h"

#endif // end of __STARMATH_H_