#ifndef CLICK_CLICKITYCLACK_STATICMEMCPY_HH
#define CLICK_CLICKITYCLACK_STATICMEMCPY_HH

#include <click/config.h>
#include <click/glue.hh>

CLICK_DECLS

namespace ClickityClack
{

template <typename T> __attribute__((always_inline)) void intTransfer(void *dst, void *src)
{
	T *dstInt = (T *)dst;
	T *srcInt = (T *)src;
	
	*dstInt = *srcInt;
}

/* general case */
template <size_t SIZE> __attribute__((always_inline)) void staticMemcpy(char *dst, char *src)
{
	staticMemcpy<8>(dst, src);
	staticMemcpy<SIZE - 8>(dst + 8, src + 8);
}

template <size_t SIZE> __attribute__((always_inline)) void staticMemcpy(void *dst, void *src)\
{
	staticMemcpy<SIZE>((char *)dst, (char *)src);
}

/* one value at a time */
template <> __attribute__((always_inline)) void staticMemcpy<8>(char *dst, char *src)
{
	intTransfer<uint64_t>(dst, src);
}

template <> __attribute__((always_inline)) void staticMemcpy<4>(char *dst, char *src)
{
	intTransfer<uint32_t>(dst, src);
}

template <> __attribute__((always_inline)) void staticMemcpy<2>(char *dst, char *src)
{
	intTransfer<uint16_t>(dst, src);
}

template <> __attribute__((always_inline)) void staticMemcpy<1>(char *dst, char *src)
{
	intTransfer<uint8_t>(dst, src);
}

template <> __attribute__((always_inline)) void staticMemcpy<0>(char *dst, char *src)
{
	(void)dst;
	(void)src;
}

/* two values at a time */
template <> __attribute__((always_inline)) void staticMemcpy<3>(char *dst, char *src)
{
	staticMemcpy<2>(dst, src);
	staticMemcpy<1>(dst + 2, src + 2);
}

template <> __attribute__((always_inline)) void staticMemcpy<5>(char *dst, char *src)
{
	staticMemcpy<4>(dst, src);
	staticMemcpy<1>(dst + 4, src + 4);
}

template <> __attribute__((always_inline)) void staticMemcpy<6>(char *dst, char *src)
{
	staticMemcpy<4>(dst, src);
	staticMemcpy<2>(dst + 4, src + 4);
}

template <> __attribute__((always_inline)) void staticMemcpy<7>(char *dst, char *src)
{
	staticMemcpy<4>(dst, src);
	staticMemcpy<3>(dst + 4, src + 4);
}

}

CLICK_ENDDECLS

#endif /* CLICK_CLICKITYCLACK_STATICMEMCPY_HH */
