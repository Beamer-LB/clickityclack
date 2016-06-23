#include "ipipencapper.hh"
#include <click/glue.hh>
#include "checksumfixup.hh"
#include "pktmemcpy.hh"

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
ELEMENT_REQUIRES(ClickityClack_PktMemCpy)
