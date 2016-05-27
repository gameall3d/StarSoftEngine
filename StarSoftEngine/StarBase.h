#include <math.h>


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