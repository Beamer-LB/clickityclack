#include "tcpoptioniterator.hh"

CLICK_DECLS

namespace ClickityClack
{

const TCPOption *TCPOptionIterator::next()
{
	while (length > 0)
	{
		const TCPOption *option = reinterpret_cast<const TCPOption *>(ptr);

		switch (option->opcode)
		{
		case TCPOPT_EOL:
			goto abort;
			
		case TCPOPT_NOP:	/* Ref: RFC 793 section 3.1 */
			ptr++;
			length--;
			continue;
			
		default:
			if (length == 1)
				goto abort;
			if (option->opsize < 2)	/* "silly options" */
				goto abort;
			if (option->opsize > length)
				goto abort;  /* don't parse partial options */
			
			ptr += option->opsize;
			length -= option->opsize;
			
			return option;
		}
	}
	
	return NULL;
	
abort:
	length = 0;
	return NULL;
}

}

CLICK_ENDDECLS

ELEMENT_PROVIDES(ClickityClack_TCPOptionIterator)
