#ifndef CLICK_ADJUSTETHER_HH
#define CLICK_ADJUSTETHER_HH

#include <click/config.h>
#include <click/element.hh>
#include <click/etheraddress.hh>
#include <click/ipaddress.hh>
#if HAVE_BATCH
#include <click/batchelement.hh>
#endif
#include "../clickityclack/lib/macint64.hh"

CLICK_DECLS

#if HAVE_BATCH
class AdjustEther: public BatchElement
#else
class AdjustEther: public Element
#endif
{
	uint64_t srcMac;
	uint64_t dstMac;
	
public:
	AdjustEther() {}
	
	~AdjustEther() {}
	
	const char *class_name() const { return "AdjustEther"; }
	
	const char *port_count() const { return "1/1"; }
	
	const char *processing() const { return AGNOSTIC; }
	
	int configure(Vector<String> &conf, ErrorHandler *errh);
	
	Packet *simple_action(Packet *p);
	
#if HAVE_BATCH
	PacketBatch *simple_action_batch(PacketBatch *head);
#endif
};

CLICK_ENDDECLS

#endif /* CLICK_ADJUSTETHER_HH */
