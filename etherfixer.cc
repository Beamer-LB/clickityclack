/*
 * arpquerier.{cc,hh} -- ARP resolver element
 * Robert Morris, Eddie Kohler
 *
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
 * Copyright (c) 2005 Regents of the University of California
 * Copyright (c) 2008-2009 Meraki, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>
#include "etherfixer.hh"
#include <clicknet/ether.h>
#include <click/etheraddress.hh>
#include <click/ipaddress.hh>
#include <click/args.hh>
#include <click/bitvector.hh>
#include <click/straccum.hh>
#include <click/router.hh>
#include <click/error.hh>
#include <click/glue.hh>
#include <click/packet_anno.hh>
CLICK_DECLS

using namespace ClickityClack;

EtherFixer::EtherFixer()
    : _arpt(0), _my_arpt(false), _zero_warned(false)
{
}

EtherFixer::~EtherFixer()
{
}

void *
EtherFixer::cast(const char *name)
{
	if (strcmp(name, "ARPTable") == 0)
		return _arpt;
	else if (strcmp(name, "EtherFixer") == 0)
		return this;
	else
		return Element::cast(name);
}

int
EtherFixer::configure(Vector<String> &conf, ErrorHandler *errh)
{
	_arpt = 0;
	if (Args(this, errh).bind(conf)
		.read("TABLE", ElementCastArg("ARPTable"), _arpt)
		.consume() < 0)
	{
		return -1;
	}
	
	if (!_arpt)
	{
		Vector<String> subconf;
		subconf.push_back("TIMEOUT 0");
		_arpt = new ARPTable;
		_arpt->attach_router(router(), -1);
		_arpt->configure(subconf, errh);
		_my_arpt = true;
	}
	
	IPAddress my_mask;
	if (conf.size() == 1)
	conf.push_back(conf[0]);
	if (Args(conf, this, errh)
		.read_mp("IP", IPPrefixArg(true), _my_ip, my_mask)
		.read_mp("ETH", _my_en)
		.complete() < 0)
	{
		return -1;
	}
	
	_my_en64 = MacInt64::getSafe(_my_en.data());
	
	if (!false)
	{
		_my_bcast_ip = _my_ip | ~my_mask;
		if (_my_bcast_ip == _my_ip)
			_my_bcast_ip = 0xFFFFFFFFU;
	}
	
	return 0;
}

int
EtherFixer::initialize(ErrorHandler *)
{
	_drops = 0;
	return 0;
}

void
EtherFixer::cleanup(CleanupStage stage)
{
	if (_my_arpt)
	{
		_arpt->cleanup(stage);
		delete _arpt;
	}
}

void
EtherFixer::take_state(Element *e, ErrorHandler *errh)
{
	EtherFixer *arpq = (EtherFixer *) e->cast("ARPQuerier");
	if (!arpq || _my_ip != arpq->_my_ip || _my_en != arpq->_my_en || _my_bcast_ip != arpq->_my_bcast_ip)
		return;

	if (_my_arpt && arpq->_my_arpt)
		_arpt->take_state(arpq->_arpt, errh);
	_drops = arpq->_drops;
}

Packet *EtherFixer::fixMACs(Packet *p)
{
	WritablePacket *wp = p->uniqueify();
	
        IPAddress dst_ip = wp->dst_ip_anno();
        EtherAddress *dst_eth = reinterpret_cast<EtherAddress *>(wp->ether_header()->ether_dhost);
        int r;
	
        r = _arpt->lookup(dst_ip, dst_eth, 0);
        if (r < 0) 
		return NULL;
	
        memcpy(&wp->ether_header()->ether_shost, _my_en.data(), 6);
	MacInt64::storeUnsafe(_my_en64, wp->ether_header()->ether_shost);
        return wp;
}

Packet *
EtherFixer::simple_action(Packet *p)
{
	Packet *result = fixMACs(p);
	
	if (!result)
	{
		++_drops;
	        p->kill();
		return NULL;
	}
	
        return result;
	
}

#if HAVE_BATCH
PacketBatch *EtherFixer::simple_action_batch(PacketBatch *head)
{
	Packet* current = head;
	Packet* last = head;
	int c = head->count();
	int dropped = 0;
	while (current != NULL)
	{
		/* do stuff */
		Packet *result = fixMACs(current);
		
		if (result)
		{
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
		else
		{
			dropped++;
			if (current == head)
			{
				head = PacketBatch::start_head(current->next());
				current->set_next(NULL);
				current->kill();
				current = head;
				last = head;
			}
			else
			{
				Packet* todrop = current;
				current = current->next();
				todrop->set_next(NULL);
				todrop->kill();
				last->set_next(current);
			}
		}
	}
	if (dropped && head)
	{
		head->set_count(c - dropped);
		head->set_tail(last);
	}
	return head;
}
#endif

String
EtherFixer::read_handler(Element *e, void *thunk)
{
	EtherFixer *q = (EtherFixer *)e;
	switch (reinterpret_cast<uintptr_t>(thunk))
	{
	case h_table:
		return q->_arpt->read_handler(q->_arpt, (void *) (uintptr_t) ARPTable::h_table);
	case h_stats:
		return String(q->_drops.value() + q->_arpt->drops()) + " packets killed\n";
	case h_count:
		return String(q->_arpt->count());
	default:
		return String();
	}
}

int
EtherFixer::write_handler(const String &str, Element *e, void *thunk, ErrorHandler *errh)
{
	EtherFixer *q = (EtherFixer *) e;
	
	switch (reinterpret_cast<uintptr_t>(thunk))
	{
	case h_insert:
		return q->_arpt->write_handler(str, q->_arpt, (void *) (uintptr_t) ARPTable::h_insert, errh);
	case h_delete:
		return q->_arpt->write_handler(str, q->_arpt, (void *) (uintptr_t) ARPTable::h_delete, errh);
	case h_clear:
		q->_arpt->clear();
		return 0;
	default:
		return -1;
	}
}

void
EtherFixer::add_handlers()
{
	add_read_handler("table", read_handler, h_table);
	add_read_handler("stats", read_handler, h_stats);
	add_read_handler("count", read_handler, h_count);
	add_data_handlers("drops", Handler::OP_READ, &_drops);
	add_data_handlers("broadcast", Handler::OP_READ | Handler::OP_WRITE, &_my_bcast_ip);
	add_data_handlers("ipaddr", Handler::OP_READ | Handler::OP_WRITE, &_my_ip);
	add_write_handler("insert", write_handler, h_insert);
	add_write_handler("delete", write_handler, h_delete);
	add_write_handler("clear", write_handler, h_clear);
}

CLICK_ENDDECLS

EXPORT_ELEMENT(EtherFixer)
ELEMENT_REQUIRES(ARPTable)
ELEMENT_MT_SAFE(EtherFixer)
