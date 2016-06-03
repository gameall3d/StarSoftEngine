#ifndef __STARVECTOR3_H_
#define __STARVECTOR3_H_

#include "StarBase.h"
#include "StarMath.h"

namespace Star
{
	class StarVector3
	{
	public:
		float32 x, y, z;

	public:
		inline StarVector3(){};
		inline StarVector3(const float32 fX, const float32 fY, const float32 fZ)
			:x(fX), y(fY), z(fZ)
		{
		}

		inline StarVector3& operator = (const StarVector3& rkVector)
		{
			x = rkVector.x;
			y = rkVector.y;
			z = rkVector.z;

			return *this;
		}
		inline StarVector3 operator + (const StarVector3& rkVector) const
		{
			return StarVector3(
				x + rkVector.x,
				y + rkVector.y,
				z + rkVector.z);
		}
		inline StarVector3 operator - (const StarVector3& rkVector) const
		{
			return StarVector3(
				x - rkVector.x,
				y - rkVector.y,
				z - rkVector.z);
		}
		inline StarVector3 operator * (const StarVector3& rkVector) const
		{
			return StarVector3(
				x * rkVector.x,
				y * rkVector.y,
				z * rkVector.z);
		}
		inline StarVector3 operator / (const StarVector3& rkVector) const
		{
			return StarVector3(
				x / rkVector.x,
				y / rkVector.y,
				z / rkVector.z);
		}
		inline StarVector3 operator - () const
		{
			return StarVector3(-x, -y, -z);
		}


		inline float32 Length() const
		{
			return StarMath::Sqrt(x*x + y*y + z*z);
		}
		inline float32 SquaredLength() const
		{
			return x*x + y*y + z*z;
		}
		inline float32 Normalise()
		{
			float32 fLength = Length();

			if (fLength > 0)
			{
				float32 fInvLength = 1.0f / fLength;
				x *= fInvLength;
				y *= fInvLength;
				z *= fInvLength;
			}

			return fLength;
		}
		inline StarVector3 NormaliseCopy() const
		{
			StarVector3 ret = *this;
			ret.Normalise();
			return ret;
		}
		inline float32 DotProduct(const StarVector3& v) const
		{
			return x * v.x + y * v.y + z * v.z;
		}
		inline StarVector3 CrossProduct(const StarVector3& rkVector) const
		{
			return StarVector3(
				y * rkVector.z - z * rkVector.y,
				z * rkVector.x - x * rkVector.z,
				x * rkVector.y - y * rkVector.x);
		}

	};

}

#endif // __STARVECTOR3_H


