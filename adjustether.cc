#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/etheraddress.hh>
#include "adjustether.hh"

CLICK_DECLS

using namespace ClickityClack;

int AdjustEther::configure(Vector<String> &conf, ErrorHandler *errh)
{
	EtherAddress srcEther;
	EtherAddress dstEther;
	
	if (Args(conf, this, errh)
		.read_m("SRC", EtherAddressArg(), srcEther)
		.read_m("DST", EtherAddressArg(), dstEther)
		.complete() < 0)
	{
		return -1;
	}
	
	srcMac = MacInt64::getSafe(srcEther.data());
	dstMac = MacInt64::getSafe(dstEther.data());
	
	return 0;
}

#if HAVE_BATCH
PacketBatch *AdjustEther::simple_action_batch(PacketBatch *head)
{
	Packet* current = head;
	Packet* last = head;
	int c = head->count();
	int dropped = 0;
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

Packet *AdjustEther::simple_action(Packet *p)
{
	WritablePacket *wp = p->uniqueify();
	
	click_ether *eth = wp->ether_header();
	
	MacInt64::storeDestructive(dstMac, eth->ether_dhost);
	MacInt64::storeUnsafe(srcMac, eth->ether_shost);
	
	return wp;
}

CLICK_ENDDECLS

EXPORT_ELEMENT(AdjustEther)
