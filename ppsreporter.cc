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

static inline void report(int thread, click_jiffies_t duration)
{
	click_chatter("%d: %d", thread, (int)duration);
}

#if HAVE_BATCH
PacketBatch *PPSReporter::simple_action_batch(PacketBatch *head)
{
	Packet *current = head;
	Packet *last = head;
	int c = head->count();
	
	while (current != NULL)
	{
		/* do stuff */
		Packet *result = simple_action(current);
		
		if (current == head)
		{
			head = PacketBatch::start_head(result);
			head->set_next(current->next());
		}
		else
		{
			last->set_next(result);
			result->set_next(current->next());
		}
		
		last = result;
		current = result->next();
	}
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

