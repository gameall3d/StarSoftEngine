#ifndef __STAR3D_H_
#define __STAR3D_H_

#include "StarBase.h"
#include "StarPrerequisites.h"
#include "StarTypes.h"

namespace Star
{
	class Star3D
	{
	public:
		Star3D();
		~Star3D();

		EStarResult CreateDevice(StarDevice** out_ppDevice, const StarDevice_Parameters* in_pDeviceParameters);
	};

	Star3D* Star3DCreate(uint32 nVersion);
}
#endif // end of __STAR3D_H_

