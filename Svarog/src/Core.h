#include <assert.h>

#ifdef SV_DEBUG
	#define ASSERT(x) assert(x);
#else
	#define ASSERT(x)
#endif
