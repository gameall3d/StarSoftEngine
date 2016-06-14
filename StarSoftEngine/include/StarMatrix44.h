#ifndef __STARMATRIX44_H_
#define __STARMATRIX44_H_

#include "StarBase.h"
#include "StarVector3.h"

namespace Star
{
	class StarMatrix44
	{
	protected:
		union 
		{
			float32 m[4][4];
			float32 _m[16];
		};

	public:
		inline StarMatrix44(){}
		inline StarMatrix44(
			float32 m00, float32 m01, float32 m02, float32 m03,
			float32 m10, float32 m11, float32 m12, float32 m13,
			float32 m20, float32 m21, float32 m22, float32 m23,
			float32 m30, float32 m31, float32 m32, float32 m33)
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;
			m[0][3] = m03;
			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;
			m[1][3] = m13;
			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
			m[2][3] = m23;
			m[3][0] = m30;
			m[3][1] = m31;
			m[3][2] = m32;
			m[3][3] = m33;
		}

		inline StarMatrix44 Concatenate(const StarMatrix44& m2) const
		{
			StarMatrix44 r;
			r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
			r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
			r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
			r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

			r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
			r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
			r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
			r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

			r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
			r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
			r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
			r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

			r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
			r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
			r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
			r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

			return r;
		}

		inline StarMatrix44 operator * (const StarMatrix44& m2) const
		{
			return Concatenate(m2);
		}

		inline float32 *operator [] (uint32 nRow)
		{
			assert(nRow < 4);
			return m[nRow];
		}

		inline const float32 *operator [] (uint32 nRow) const
		{
			assert(nRow < 4);
			return m[nRow];
		}

		// for trans matrix
		inline void SetTrans(const StarVector3& v)
		{
			m[0][3] = v.x;
			m[1][3] = v.y;
			m[2][3] = v.z;
		}

		inline void MakeTrans(const StarVector3& v)
		{
			m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = v.x;
			m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = v.y;
			m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = v.z;
			m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
		}

		inline void MakeTrans(float32 tx, float32 ty, float32 tz)
		{
			m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = tx;
			m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = ty;
			m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = tz;
			m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
		}

		inline static StarMatrix44 BuildTrans(const StarVector3& v)
		{
			StarMatrix44 r;

			r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = v.x;
			r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = v.y;
			r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = v.z;
			r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

			return r;
		}

		inline static StarMatrix44 BuildTrans(float32 tx, float32 ty, float32 tz)
		{
			StarMatrix44 r;

			r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = tx;
			r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = ty;
			r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = tz;
			r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

			return r;
		}

		// for scale matrix
		inline void SetScale(const StarVector3& v)
		{
			m[0][0] = v.x;
						  m[1][1] = v.y;
										m[2][2] = v.z;
		}

		inline static StarMatrix44 BuildScale(const StarVector3& v)
		{
			StarMatrix44 r;
			r.m[0][0] = v.x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
			r.m[1][0] = 0.0; r.m[1][1] = v.y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
			r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = v.z; r.m[2][3] = 0.0;
			r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

			return r;
		}

		inline static StarMatrix44 BuildScale(float32 sx, float32 sy, float32 sz)
		{
			StarMatrix44 r;
			r.m[0][0] = sx;  r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
			r.m[1][0] = 0.0; r.m[1][1] = sy;  r.m[1][2] = 0.0; r.m[1][3] = 0.0;
			r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = sz;  r.m[2][3] = 0.0;
			r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

			return r;
		}

		inline static StarMatrix44 BuildMatrixLookAtLH(const StarVector3& eye, const StarVector3& at, const StarVector3& up)
		{
			const StarVector3 vZAxis = (at - eye).NormaliseCopy();
			StarVector3 vXAxis = up.CrossProduct(vZAxis).NormaliseCopy();
			StarVector3 vYAxis = vZAxis.CrossProduct(vXAxis);

			StarMatrix44 r;
			r.m[0][0] = vXAxis.x; r.m[0][1] = vYAxis.x; r.m[0][2] = vZAxis.x; r.m[0][3] = 0.0;
			r.m[1][0] = vXAxis.y; r.m[1][1] = vYAxis.y; r.m[1][2] = vZAxis.y; r.m[1][3] = 0.0;
			r.m[2][0] = vXAxis.z; r.m[2][1] = vYAxis.z; r.m[2][2] = vZAxis.z; r.m[2][3] = 0.0;
			r.m[3][0] = -vXAxis.DotProduct(eye); r.m[3][1] = -vYAxis.DotProduct(eye); r.m[3][2] = -vZAxis.DotProduct(eye); r.m[3][3] = 1.0;

			return r;
		}

		inline static StarMatrix44 BuildMatrixPerspectiveFOVLH(const float32 fFOV, const float32 fAspect, const float32 fZNear, const float32 fZFar)
		{
			const float32 fCotangentHalfFOV = 1.0f / tanf(fFOV * 0.5f);
			
			StarMatrix44 r;
			r.m[0][0] = fCotangentHalfFOV / fAspect;  r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
			r.m[1][0] = 0.0; r.m[1][1] = fCotangentHalfFOV;  r.m[1][2] = 0.0; r.m[1][3] = 0.0;
			r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = fZFar / (fZFar - fZNear);  r.m[2][3] = 1.0;
			r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = fZNear * fZFar / (fZNear - fZFar); r.m[3][3] = 0;

			return r;
		}

		static const StarMatrix44 ZERO;
		static const StarMatrix44 IDENTITY;
	};


	inline StarVector4 operator * (const StarVector4&v, const StarMatrix44& mat)
	{
		return StarVector4(
			v.x*mat[0][0] + v.y*mat[1][0] + v.z*mat[2][0] + v.w*mat[3][0],
			v.x*mat[0][1] + v.y*mat[1][1] + v.z*mat[2][1] + v.w*mat[3][1],
			v.x*mat[0][2] + v.y*mat[1][2] + v.z*mat[2][2] + v.w*mat[3][2],
			v.x*mat[0][3] + v.y*mat[1][3] + v.z*mat[2][3] + v.w*mat[3][3]
			);
	}
}

#endif // __STARMATRIX4_H_

