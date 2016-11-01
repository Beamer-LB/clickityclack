#ifndef CLICK_CYCLEMUNCHER_HH
#define CLICK_CYCLEMUNCHER_HH

#include <click/config.h>
#include <click/element.hh>
#if HAVE_BATCH
#include <click/batchelement.hh>
#endif

CLICK_DECLS

#if HAVE_BATCH
class CycleMuncher: public BatchElement
#else
class CycleMuncher: public Element
#endif
{
	long long flatCycles;
	long long perByteCycles;
	
public:
	CycleMuncher()
		: flatCycles(0), perByteCycles(0) {}
	
	~CycleMuncher() {}
	
	const char *class_name() const { return "CycleMuncher"; }
	
	const char *port_count() const { return "1-/="; }
	
	const char *processing() const { return AGNOSTIC; }
	
	int configure(Vector<String> &conf, ErrorHandler *errh);
	
	Packet *simple_action(Packet *p);
	
#if HAVE_BATCH
	PacketBatch *simple_action_batch(PacketBatch *head);
#endif
};

CLICK_ENDDECLS

#endif /* CLICK_CYCLEMUNCHER_HH */
