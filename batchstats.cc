#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/args.hh>
#include "batchstats.hh"

CLICK_DECLS

int BatchStats::configure(Vector<String> &conf, ErrorHandler *errh)
{
	maxBatch = 1024;
	reportInterval = 5;
	
	if (Args(conf, this, errh)
		.read("MAX_BATCH",       IntArg(), maxBatch)
		//.read("REPORT_INTERVAL", IntArg(), reportInterval)
		.complete() < 0)
	{
		return -1;
	}
	
	counts = new uint64_t[maxBatch + 1]; assert(counts);
	for (int i = 0; i <= maxBatch; i++)
		counts[i] = 0;
	
	return 0;
}

int BatchStats::initialize(ErrorHandler *errh)
{
	(void)errh;
	
	//timer.initialize(this);
	//timer.schedule_after_sec(reportInterval);
	
	return 0;
}

#if HAVE_BATCH
PacketBatch *BatchStats::simple_action_batch(PacketBatch *head)
{
	counts[head->count()]++;
	
	return head;
}
#endif

Packet *BatchStats::simple_action(Packet *p)
{
	(void)p;
	
	counts[1]++;
}

void BatchStats::timerHook(Timer *timer, void *userData)
{
	BatchStats *me = reinterpret_cast<BatchStats *>(userData);
	
	click_jiffies_t now = click_jiffies();
	
	for (int i = 1; i <= me->maxBatch; i++)
		click_chatter("BatchStats: time = %u size = %d count = %llu", now, i, me->counts[i]);
	
	timer->schedule_after_sec(me->reportInterval);
}

void BatchStats::cleanup(CleanupStage stage)
{
	(void)stage;
	
	for (int i = 1; i <= me->maxBatch; i++)
		click_chatter("BatchStats: size = %d count = %llu", i, me->counts[i]);
}

CLICK_ENDDECLS

EXPORT_ELEMENT(BatchStats)
