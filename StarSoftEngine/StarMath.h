#ifndef __STARMATH_H_
#define __STARMATH_H_
//#include "StarBase.h"
#include <assert.h>
#include <algorithm>
#include "StarPrerequisites.h"
#include "StarColor.h"


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

		static inline float32 Interpolate(float32 fMin, float32 fMax, float32 fGradient)
		{
			return fMin + (fMax - fMin) * Clamp<float32>(fGradient, 0, 1);
		}

		static inline StarColor Interpolate(const StarColor& StartColor, const StarColor& EndColor, float32 fGradient)
		{
			return StartColor + (EndColor - StartColor) * Clamp<float32>(fGradient, 0, 1);
		}
	};
}

#include "StarVector2.h"
#include "StarVector3.h"
#include "StarVector4.h"

#endif // end of __STARMATH_H_