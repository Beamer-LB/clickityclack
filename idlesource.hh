#ifndef CLICK_IDLESOURCE_HH
#define CLICK_IDLESOURCE_HH

#include <click/config.h>
#include <click/element.hh>
#if HAVE_BATCH
#include <click/batchelement.hh>
#endif


CLICK_DECLS

#if HAVE_BATCH
class IdleSource: public BatchElement
#else
class IdleSource: public Element
#endif
{
public:
	IdleSource() {}

	~IdleSource() {}

	const char *class_name() const { return "IdleSource"; }

	const char *port_count() const { return "0/1-"; }

	const char *processing() const { return PUSH; }
};

CLICK_ENDDECLS

#endif /* CLICK_IDLESOURCE_HH */
