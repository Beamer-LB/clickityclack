#ifndef CLICK_CLICKITYCLACK_HTONLL_HH
#define CLICK_CLICKITYCLACK_HTONLL_HH

#include <click/config.h>
#include <click/glue.hh>

#ifdef CLICK_USERLEVEL
#include <byteswap.h>
#endif

CLICK_DECLS

namespace ClickityClack
{

inline uint64_t htonll(uint64_t num)
{
#if CLICK_BYTE_ORDER == CLICK_LITTLE_ENDIAN

#ifdef CLICK_USERLEVEL
	return bswap_64(num);
#else
#error "64-bit byteswap unsupported"
#endif

#elif CLICK_BYTE_ORDER == CLICK_BIG_ENDIAN
	return num;
#endif
}

inline uint64_t ntohll(uint64_t num)
{
	return htonll(num);
}

}

CLICK_ENDDECLS

#endif // CLICK_CLICKITYCLACK_HTONLL_HH
