#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/args.hh>
#include "ppsreporter.hh"

CLICK_DECLS

int PPSReporter::configure(Vector<String> &conf, ErrorHandler *errh)
{
	epochSize = 1000000;
	
	seen = new int[click_max_cpu_ids()]; assert(seen);
	lastEpoch = new click_jiffies_t[click_max_cpu_ids()]; assert(lastEpoch);
	for (int i = 0; i < click_max_cpu_ids(); i++)
	{
		seen[i] = 0;
		lastEpoch[i] = click_jiffies();
	}
	
	if (Args(conf, this, errh)
		.read("EPOCH", IntArg(), epochSize)
		.complete() < 0)
	{
		return -1;
	}
	
	return 0;
}

#if HAVE_BATCH
PacketBatch *PPSReporter::simple_action_batch(PacketBatch *head)
{
	int thread = click_current_cpu_id();
	
	seen[thread] += head->count();
	
	if (unlikely(seen[thread] >= epochSize))
		newEpoch(thread);
	
	return head;
}
#endif

Packet *PPSReporter::simple_action(Packet *p)
{
	int thread = click_current_cpu_id();
	
	seen[thread]++;
	
	if (unlikely(seen[thread] >= epochSize))
		newEpoch(thread);
	
	return p;
}

CLICK_ENDDECLS

EXPORT_ELEMENT(PPSReporter)

