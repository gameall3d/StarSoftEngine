#ifndef __STARCOLOR_H_
#define __STARCOLOR_H_

#include "StarBase.h"
#include "StarPrerequisites.h"

namespace Star
{
	typedef uint32 RGBA;
	typedef uint32 ARGB;
	typedef uint32 ABGR;
	typedef uint32 BGRA;


	class StarColor
	{
	public:
		float r, g, b, a;

		static const StarColor ZERO;
		static const StarColor Black;
		static const StarColor White;
		static const StarColor Red;
		static const StarColor Green;
		static const StarColor Blue;

	public:
		StarColor(float red = 1.0f,
			float green = 1.0f,
			float blue = 1.0f,
			float alpha = 1.0f) :r(red), g(green), b(blue), a(alpha)
		{

		}

		inline StarColor operator + (const StarColor& rkColor) const
		{
			StarColor kSum;

			kSum.r = r + rkColor.r;
			kSum.g = g + rkColor.g;
			kSum.b = b + rkColor.b;
			kSum.a = a + rkColor.a;

			return kSum;
		}

		inline StarColor operator - (const StarColor& rkColor) const
		{
			StarColor kDiff;

			kDiff.r = r - rkColor.r;
			kDiff.g = g - rkColor.g;
			kDiff.b = b - rkColor.b;
			kDiff.a = a - rkColor.a;

			return kDiff;
		}

		inline StarColor operator * (const float fScalar) const
		{
			StarColor kProd;

			kProd.r = fScalar*r;
			kProd.g = fScalar*g;
			kProd.b = fScalar*b;
			kProd.a = fScalar*a;

			return kProd;
		}

		inline StarColor operator / (const float fScalar) const
		{
			assert(fScalar != 0.0);

			StarColor kDiv;

			float fInv = 1.0f / fScalar;
			kDiv.r = r * fInv;
			kDiv.g = g * fInv;
			kDiv.b = b * fInv;
			kDiv.a = a * fInv;

			return kDiv;
		}

		inline StarColor& operator += (const StarColor& rkColor)
		{
			r += rkColor.r;
			g += rkColor.g;
			b += rkColor.b;
			a += rkColor.a;

			return *this;
		}

		inline StarColor& operator *= (const float fScalar)
		{
			r *= fScalar;
			g *= fScalar;
			b *= fScalar;
			a *= fScalar;

			return *this;
		}

		RGBA GetAsRGBA(void) const;
		ARGB GetAsARGB(void) const;

		void SetAsRGBA(const RGBA val);
		void SetAsARGB(const ARGB val);
	};


}

#endif // end of __STARCOLOR_H_

