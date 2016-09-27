#ifndef CLICK_PPSREPORTER_HH
#define CLICK_PPSREPORTER_HH

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
class PPSReporter: public BatchElement
#else
class PPSReporter: public Element
#endif
{
	int epochSize;
	
	int *seen;
	click_jiffies_t *lastEpoch;
	
	inline void newEpoch(int thread)
	{
		click_jiffies_t now = click_jiffies();
		
		seen[thread] -= epochSize;
		click_chatter("%d: %d", thread, (int)(lastEpoch[thread] - now));
		lastEpoch[thread] = now;
	}
	
public:
	PPSReporter() {}
	
	~PPSReporter() {}
	
	const char *class_name() const { return "PPSReporter"; }
	
	const char *port_count() const { return "1-/="; }
	
	const char *processing() const { return AGNOSTIC; }
	
	int configure(Vector<String> &conf, ErrorHandler *errh);
	
	Packet *simple_action(Packet *p);
	
#if HAVE_BATCH
	PacketBatch *simple_action_batch(PacketBatch *head);
#endif
};

CLICK_ENDDECLS

#endif /* CLICK_PPSREPORTER_HH */
