#include <math.h>


enum EStarResult
{
	SR_OK = 0,
	SR_FAILED,
	SR_UKNOWN
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

// types define
enum  EColorFormat
{
	CFMT_R32,
	CFMT_R32G32,
	CFMT_R32G32B32,
	CFMT_R32G32B32A32
};

/// Outputs an error-description in case a function fails, only available in debug-builds.
/// @param[in] errdesc error-description, c-string.
#define FUNC_FAILING( errdesc ) { OutputDebugString( errdesc ); }