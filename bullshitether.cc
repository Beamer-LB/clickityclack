#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <click/args.hh>
#include <clicknet/ether.h>
#include <click/etheraddress.hh>
#include "bullshitether.hh"

CLICK_DECLS

using namespace ClickityClack;

int BullshitEther::configure(Vector<String> &conf, ErrorHandler *errh)
{
	EtherAddress srcEther;
	EtherAddress ether1;
	EtherAddress ether2;
	
	if (Args(conf, this, errh)
		.read_m("SRC",  EtherAddressArg(), srcEther)
		.read_m("IP1",  IPAddressArg(), ip1)
		.read_m("IP2",  IPAddressArg(), ip2)
		.read_m("MAC1", EtherAddressArg(), ether1)
		.read_m("MAC2", EtherAddressArg(), ether2)
		.complete() < 0)
	{
		return -1;
	}
	
	srcMac = MacInt64::getSafe(srcEther.data());
	mac1 = MacInt64::getSafe(ether1.data());
	mac2 = MacInt64::getSafe(ether2.data());
	
	return 0;
}

#if HAVE_BATCH
PacketBatch *BullshitEther::simple_action_batch(PacketBatch *head)
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

Packet *BullshitEther::simple_action(Packet *p)
{
	WritablePacket *wp = p->uniqueify();
	
	click_ether *eth = wp->ether_header();
	click_ip *iph = wp->ip_header();
	
	if (iph->ip_dst == ip1.addr())
		MacInt64::storeDestructive(mac1, eth->ether_dhost);
	else //if (iph->ip_dst == ip2.addr())
		MacInt64::storeDestructive(mac2, eth->ether_dhost);
	
	MacInt64::storeUnsafe(srcMac, eth->ether_shost);
	
	return wp;
}

CLICK_ENDDECLS

EXPORT_ELEMENT(BullshitEther)
