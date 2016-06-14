
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

		inline StarVector2& operator = (const StarVector2& rkVector)
		{
			x = rkVector.x;
			y = rkVector.y;

			return *this;
		}

		inline StarVector2 operator + (const StarVector2& rkVector) const
		{
			return StarVector2(
				x + rkVector.x,
				y + rkVector.y);
		}

		inline StarVector2 operator - (const StarVector2& rkVector) const
		{
			return StarVector2(
				x - rkVector.x,
				y - rkVector.y);
		}

		inline StarVector2 operator * (const float32 fScalar) const
		{
			return StarVector2(
				x * fScalar,
				y * fScalar);
		}

		inline StarVector2 operator / (const float32 fScalar) const
		{
			assert(fScalar != 0.0);

			float32 fInv = 1.0f / fScalar;

			return StarVector2(
				x * fInv,
				y * fInv);
		}

		inline StarVector2 operator += (const StarVector2& rkVector)
		{
			x += rkVector.x;
			y += rkVector.y;

			return *this;
		}

		static const StarVector2 ZERO;

	};
}


#endif