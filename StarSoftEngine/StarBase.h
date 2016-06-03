#ifndef __STARBASE_H_
#define __STARBASE_H_
#include <math.h>

#ifndef WIN32
#define  OutputDebugString printf
#else
#include <windows.h>
#endif

namespace Star
{


	// Integer formats of fixed bit width
	typedef unsigned int uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;
	typedef int int32;
	typedef short int16;
	typedef char int8;

	typedef float float32;

	enum EStarResult
	{
		SR_OK = 0,
		SR_FAILED,
		SR_UKNOWN,
		SR_INVALIDPARAM
	};


	inline int ftol(float f)
	{
		static int tmp;

#if _MSC_VER > 1000
		__asm
		{
			fld f
				fistp tmp
		}
#else
		asm volatile("fistpl %0" : "=m" (tmp) : "t" (f) : "st");
#endif

		return tmp;
	}


/// Outputs an error-description in case a function fails, only available in debug-builds.
/// @param[in] errdesc error-description, c-string.
#define FUNC_FAILING( errdesc ) { OutputDebugString( errdesc ); }

/// Macro to test for success.
/// @param[in] res a function return value.
/// @return true if the function succeeded.
#define FUNC_SUCCESSFUL( res )	( res == SR_OK )

/// Macro to test for failure.
/// @param[in] res a function return value.
/// @return true if the function failed.
#define FUNC_FAILED( res )		( res != SR_OK )

#define SAFE_DELETE(p)	{ if(p) {delete p; p = 0; }}
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] p; p = 0; }}
#define SAFE_RELEASE(p)	{ if(p) { (p)->Release(); p = 0; }}

}
#endif // end of __STARBASE_H_




