#ifndef CLICK_CLICKITYCLACK_TCPOPTIONITERATOR_HH
#define CLICK_CLICKITYCLACK_TCPOPTIONITERATOR_HH

#include <click/config.h>
#include <click/glue.hh>
#include <clicknet/tcp.h>

CLICK_DECLS

namespace ClickityClack
{

struct TCPOption
{
	uint8_t opcode;
	uint8_t opsize;
} __attribute__((packed));

class TCPOptionIterator
{
	const unsigned char *ptr;
	int length;
	
public:
	TCPOptionIterator(const click_tcp *tcpHeader)
		: ptr(reinterpret_cast<const unsigned char *>(tcpHeader + 1)), length((tcpHeader->th_off * 4) - sizeof(struct click_tcp)) {}
	
	const TCPOption *next();
};

}

CLICK_ENDDECLS

#endif /* CLICK_CLICKITYCLACK_TCPOPTIONITERATOR_HH */
