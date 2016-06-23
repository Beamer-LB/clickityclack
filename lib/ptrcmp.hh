#ifndef CLICK_CLICKITYCLACK_PTRCMP_HH
#define CLICK_CLICKITYCLACK_PTRCMP_HH

#include <click/config.h>
#include <click/glue.hh>

CLICK_DECLS

namespace ClickityClack
{

int ptrCmp(const void *aPtr, const void *bPtr)
{
	uint64_t a = reinterpret_cast<uint64_t>(aPtr);
	uint64_t b = reinterpret_cast<uint64_t>(bPtr);
	
	if (a < b)
		return -1;
	if (a > b)
		return 1;
	return 0;
}

}

CLICK_ENDDECLS

#endif /* CLICK_CLICKITYCLACK_PTRCMP_HH */
