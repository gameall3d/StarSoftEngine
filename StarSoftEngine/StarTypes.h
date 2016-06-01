#ifndef __STARTYPES_H_
#define __STARTYPES_H_

#include "StarBase.h"

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN		// speed up	generate process, say no to MFC
typedef HWND WindowHandle;

#endif

namespace Star
{
	struct StarDevice_Parameters
	{
		uint32 nBackBufferWidth, nBackBufferHeight;

		WindowHandle hDeviceWindow;
		bool		 bWindowed;
	};
}

#endif // end of __STARTYPES_H_