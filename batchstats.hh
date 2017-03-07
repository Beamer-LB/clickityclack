#ifndef CLICK_BATCHSTATS_HH
#define CLICK_BATCHSTATS_HH

#include <click/config.h>
#include <click/element.hh>
#if HAVE_BATCH
#include <click/batchelement.hh>
#endif


CLICK_DECLS

/* This is NOT thread-safe */

#if HAVE_BATCH
class BatchStats: public BatchElement
#else
class BatchStats: public Element
#endif
{
protected:
	uint64_t *counts;
	int maxBatch;
	int reportInterval;
	
	Timer timer;
	
	static void timerHook(Timer *timer, void *userData);
	
public:
	BatchStats()
		: counts(NULL), timer(timerHook, this) {}
	
	~BatchStats()
	{
		if (counts)
			delete counts;
	}
	
	const char *class_name() const { return "BatchStats"; }
	
	const char *port_count() const { return "1/1"; }
	
	const char *processing() const { return AGNOSTIC; }
	
	int configure(Vector<String> &conf, ErrorHandler *errh);
	
	int initialize(ErrorHandler *errh);
	
	Packet *simple_action(Packet *p);
	
#if HAVE_BATCH
	PacketBatch *simple_action_batch(PacketBatch *head);
#endif
};

CLICK_ENDDECLS

#endif /* CLICK_BATCHSTATS_HH */
