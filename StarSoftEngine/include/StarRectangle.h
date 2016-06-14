#ifndef __STARRECTANGLE_H_
#define __STARRECTANGLE_H_
#include "StarBase.h"

namespace Star
{
	class StarRectangle
	{
	public:
		StarRectangle();
		~StarRectangle();

	public:
		int32 nLeft, nTop;
		int32 nRight, nBottom;
	};

}

#endif // __STARRECTANGLE_H_

