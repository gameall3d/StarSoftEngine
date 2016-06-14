#include "Star3D.h"
#include "StarDevice.h"

namespace Star
{
	Star3D::Star3D()
	{
	}


	Star3D::~Star3D()
	{
	}

	EStarResult Star3D::CreateDevice(StarDevice** out_ppDevice, const StarDevice_Parameters* in_pDeviceParameters)
	{
		if (!out_ppDevice)
		{
			// wait to add log
			return SR_INVALIDPARAM;
		}

		if (!in_pDeviceParameters)
		{
			*out_ppDevice = 0;
			return SR_INVALIDPARAM;
		}

		*out_ppDevice = new StarDevice(this, in_pDeviceParameters);

		if (!(*out_ppDevice))
		{
			return SR_FAILED;
		}

		return (*out_ppDevice)->Create();
	}

	Star3D* Star3DCreate(uint32 nVersion)
	{
		Star3D* pStar3D = new Star3D();

		return pStar3D;
	}
}
