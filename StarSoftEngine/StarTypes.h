#ifndef __STARTYPES_H_
#define __STARTYPES_H_

#include "StarBase.h"
#include "StarMath.h"
#include "StarColor.h"

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

	// types define
	enum  EColorFormat
	{
		CFMT_R32,
		CFMT_R32G32,
		CFMT_R32G32B32,
		CFMT_R32G32B32A32
	};

	enum EStarTransformStateType
	{
		STST_WORLD,
		STST_VIEW,
		STST_PROJECTION,

		STST_NUM
	};

	struct StarVertexData
	{
		StarVector4 pos;
		StarColor color;
	};

	struct StarScanLineVertexData
	{
		uint32 nXPos;
		float32 fZPos;
		StarColor color;
	};

	struct StarVSOutput
	{
		StarVector4 vPosition;
	};
}

#endif // end of __STARTYPES_H_