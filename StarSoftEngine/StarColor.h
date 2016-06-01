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
		explicit StarColor(float red = 1.0f,
			float green = 1.0f,
			float blue = 1.0f,
			float alpha = 1.0f) :r(red), g(green), b(blue), a(alpha)
		{

		}


		RGBA GetAsRGBA(void) const;
		ARGB GetAsARGB(void) const;

		void SetAsRGBA(const RGBA val);
		void SetAsARGB(const ARGB val);
	};


}

#endif // end of __STARCOLOR_H_

