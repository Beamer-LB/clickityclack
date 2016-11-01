#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/etheraddress.hh>
#include "cyclemuncher.hh"

CLICK_DECLS

int CycleMuncher::configure(Vector<String> &conf, ErrorHandler *errh)
{
	double flatCyclesD = 0;
	double perByteCyclesD = 0;
	
	if (Args(conf, this, errh)
		.read_m("FLAT",     DoubleArg(), flatCyclesD)
		.read_m("PER_BYTE", DoubleArg(), perByteCyclesD)
		.complete() < 0)
	{
		return -1;
	}
	
	flatCycles = flatCyclesD;
	perByteCycles = perByteCyclesD;
	
	return 0;
}

#if HAVE_BATCH
PacketBatch *CycleMuncher::simple_action_batch(PacketBatch *head)
{
	Packet *current = head;
	Packet *last = head;
	
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

Packet *CycleMuncher::simple_action(Packet *p)
{
	for (long long i = 0; i < flatCycles + perByteCycles * p->length(); i++)
		asm("");
	
	return p;
}

CLICK_ENDDECLS

EXPORT_ELEMENT(CycleMuncher)
