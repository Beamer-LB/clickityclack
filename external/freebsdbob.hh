/*
 * Shamelessly copied from FreeBSD
 */

#ifndef CLICK_CLICKITYCLACK_FREEBSDBOB_HH
#define CLICK_CLICKITYCLACK_FREEBSDBOB_HH

#include <click/config.h>
#include <click/glue.hh>

CLICK_DECLS

namespace ClickityClack
{

/* ----- FreeBSD if_bridge hash function ------- */

/*
* The following hash function is adapted from "Hash Functions" by Bob Jenkins
* ("Algorithm Alley", Dr. Dobbs Journal, September 1997).
*
* http://www.burtleburtle.net/bob/hash/spooky.html
*/

#define CLICK_CLICKITYCLACK_BOB_MIX(a, b, c)                                      \
do {                                                                    \
	a -= b; a -= c; a ^= (c >> 13);                                 \
	b -= c; b -= a; b ^= (a << 8);                                  \
	c -= a; c -= b; c ^= (b >> 13);                                 \
	a -= b; a -= c; a ^= (c >> 12);                                 \
	b -= c; b -= a; b ^= (a << 16);                                 \
	c -= a; c -= b; c ^= (b >> 5);                                  \
	a -= b; a -= c; a ^= (c >> 3);                                  \
	b -= c; b -= a; b ^= (a << 10);                                 \
	c -= a; c -= b; c ^= (b >> 15);                                 \
} while (/*CONSTCOND*/0)

static inline uint32_t freeBSDBob(uint32_t target1, uint16_t target2 = 0, uint16_t target3 = 0)
{
	uint32_t a = 0x9e3779b9, b = 0x9e3779b9, c = 0; // hask key
	
	b += (uint32_t)target3 << 16;
	b += target2;
	a += target1;	
	CLICK_CLICKITYCLACK_BOB_MIX(a, b, c);
	return c;
}

#undef CLICK_CCLICKITYCLACK_BOB_MIX

}

CLICK_ENDDECLS

#endif /* CLICK_CLICKITYCLACK_FREEBSDBOB_HH */
