#include "ipipencapper.hh"
#include <click/glue.hh>
#include "checksumfixup.hh"

CLICK_DECLS

namespace ClickityClack
{

IPIPEncapper::IPIPEncapper(uint8_t ttl)
{
	memset(&iph, 0, sizeof(click_ip));
	iph.ip_v = 4;
	iph.ip_hl = sizeof(click_ip) >> 2;
	iph.ip_ttl = ttl;
	iph.ip_p = IPPROTO_IPIP;
#if HAVE_FAST_CHECKSUM
	iph.ip_sum = ip_fast_csum((unsigned char *)&iphPlain, sizeof(click_ip) >> 2);
#else
	iph.ip_sum = click_in_cksum((unsigned char *)&iph, sizeof(click_ip));
#endif
}

#if 0
static void moveMem(unsigned char *src, unsigned char *dst, ssize_t len)
{
	size_t leftoverLen = len % 16;
	size_t len64 = (len - leftoverLen) / 8;
	
	if (likely(len64))
		moveMem64(reinterpret_cast<uint64_t *>(src + leftoverLen), reinterpret_cast<uint64_t *>(dst + leftoverLen), len64);
	
	if (unlikely(!leftoverLen))
		return;
	
	do
	{
		len--;
		dst[len] = src[len];
	}
	while (likely(len >= 0));
}
#endif

/* for len >= 1 */
static void memcpyFast64(uint64_t *dst, uint64_t *src, ssize_t len)
{
	int i = 0;
	
	do
	{
		dst[i] = src[i];
		i++;
	}
	while (likely(i < len));
}

/* for len >= 16 */
static void memcpyFast(unsigned char *dst, unsigned char *src, ssize_t len)
{
	ssize_t leftoverLen = len % 8;
	ssize_t len64 = (len - leftoverLen) / 8;
	
	memcpyFast64(reinterpret_cast<uint64_t *>(dst + leftoverLen), reinterpret_cast<uint64_t *>(src + leftoverLen), len64);
	
	if (unlikely(!leftoverLen))
		return;
	
	int i = 0;
	
	do
	{
		dst[i] = src[i];
		i++;
	}
	while (likely(i < leftoverLen));
}

/* multiples of 8 only */
static void moveMemBulk(uint64_t *src, uint64_t *dst, ssize_t len)
{
	if (len % 8)
		len = (len / 8 + 1) * 8;
	
	do
	{ 
		dst[len - 1] = src[len - 1];
		dst[len - 2] = src[len - 2];
		dst[len - 3] = src[len - 3];
		dst[len - 4] = src[len - 4];
		dst[len - 5] = src[len - 5];
		dst[len - 6] = src[len - 6];
		dst[len - 7] = src[len - 7];
		dst[len - 8] = src[len - 8];
		len -= 8;
	}
	while (likely(len > 0));
}
	

WritablePacket *IPIPEncapper::encapsulate(Packet *p, uint32_t src, uint32_t dst)
{
	size_t displaceLen = p->end_data() - p->network_header();
	size_t displaceDWords = displaceLen / 8 + (displaceLen % 8 ? 1 : 0);
	WritablePacket *wp = p->put(sizeof(click_ip));
	size_t oldIPLen = ntohs(wp->ip_header()->ip_len);
	
	if (!wp)
		return 0;
	
	/* make room */
	moveMemBulk(reinterpret_cast<uint64_t *>(wp->network_header()), reinterpret_cast<uint64_t *>(wp->network_header() + sizeof(click_ip)), displaceDWords);
	
	click_ip *ip = wp->ip_header();
	
	memcpyFast(reinterpret_cast<unsigned char *>(ip), reinterpret_cast<unsigned char *>(&iph), sizeof(click_ip));
	
	ip->ip_src.s_addr = src;
	ip->ip_dst.s_addr = dst;
	ip->ip_len = htons(oldIPLen + sizeof(click_ip));
	
	ip->ip_sum = checksumFold(
		checksumFixup32(0, src,
		checksumFixup32(0, dst,
		checksumFixup16(0, ip->ip_len,
		ip->ip_sum))));
	
	wp->set_ip_header(ip, sizeof(click_ip));
	
	return wp;
}

}

CLICK_ENDDECLS

ELEMENT_PROVIDES(ClickityClack_IPIPEncapper)

