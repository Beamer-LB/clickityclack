#ifndef CLICK_BULLSHITETHER_HH
#define CLICK_BULLSHITETHER_HH

#include <click/config.h>
#include <click/element.hh>
#include <click/etheraddress.hh>
#include <click/ipaddress.hh>
#if HAVE_BATCH
#include <click/batchelement.hh>
#endif
#include "lib/macint64.hh"

CLICK_DECLS

#if HAVE_BATCH
class BullshitEther: public BatchElement
#else
class BullshitEther: public Element
#endif
{
	uint64_t srcMac;
	IPAddress ip1;
	IPAddress ip2;
	uint64_t mac1;
	uint64_t mac2;
	
public:
	BullshitEther() {}
	
	~BullshitEther() {}
	
	const char *class_name() const { return "BullshitEther"; }
	
	const char *port_count() const { return "1-/="; }
	
	const char *processing() const { return AGNOSTIC; }
	
	int configure(Vector<String> &conf, ErrorHandler *errh);
	
	Packet *simple_action(Packet *p);
	
#if HAVE_BATCH
	PacketBatch *simple_action_batch(PacketBatch *head);
#endif
};

CLICK_ENDDECLS

#endif /* CLICK_BULLSHITETHER_HH */
