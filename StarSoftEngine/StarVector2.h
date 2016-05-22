
#ifndef __Vector2_H_
#define __Vector2_H_

namespace Star
{
	class Vector2
	{
	public:
		float x, y;

	public:
		inline Vector2()
		{

		}

		inline Vector2(const float fX, const float fY)
			:x(fX), y(fY)
		{

		}
	};
}


#endif