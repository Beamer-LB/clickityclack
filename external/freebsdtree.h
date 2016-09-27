/*
 * Shamelessly copied from FreeBSD 9.2 (sys/sys/tree.h)
 *
 * ClickityClack-specific changes:
 *  * removed splay tree
 *  * replaced "struct type" with "type" everywhere
 *  * <sys/cdefs.h> not included
 *  * changed include guards
 *  * got rid of CLICK_CLICKITYCLACK_RB_INITIALIZER's useless argument (TODO: maybe go upstream?)
 *  * replaced "__unused" with __attribute__((__unused__))
 *  * always access elem->*field, instead of elem->field (template-friendly)
 *  * prefixed all macros with CLICK_CLICKITYCLACK_
 */


/*	$NetBSD: tree.h,v 1.8 2004/03/28 19:38:30 provos Exp $	*/
/*	$OpenBSD: tree.h,v 1.7 2002/10/17 21:51:54 art Exp $	*/
/* $FreeBSD: release/9.2.0/sys/sys/tree.h 189204 2009-03-01 04:57:23Z bms $ */

/*-
 * Copyright 2002 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef	CLICK_CLICKITYCLACK_FREEBSDTREE_H
#define	CLICK_CLICKITYCLACK_FREEBSDTREE_H

/*
 * This file defines data structures for different types of trees:
 * splay trees and red-black trees.
 *
 * A splay tree is a self-organizing data structure.  Every operation
 * on the tree causes a splay to happen.  The splay moves the requested
 * node to the root of the tree and partly rebalances it.
 *
 * This has the benefit that request locality causes faster lookups as
 * the requested nodes move to the top of the tree.  On the other hand,
 * every lookup causes memory writes.
 *
 * The Balance Theorem bounds the total access time for m operations
 * and n inserts on an initially empty tree as O((m + n)lg n).  The
 * amortized cost for a sequence of m accesses to a splay tree is O(lg n);
 *
 * A red-black tree is a binary search tree with the node color as an
 * extra attribute.  It fulfills a set of conditions:
 *	- every search path from the root to a leaf consists of the
 *	  same number of black nodes,
 *	- each red node (except for the root) has a black parent,
 *	- each leaf node is black.
 *
 * Every operation on a red-black tree is bounded as O(lg n).
 * The maximum height of a red-black tree is 2lg (n+1).
 */

/* ***SNIP*** */

/* Macros that define a red-black tree */
#define CLICK_CLICKITYCLACK_RB_HEAD(name, type)						\
struct name {								\
	type *rbh_root; /* root of the tree */			\
}

#define CLICK_CLICKITYCLACK_RB_INITIALIZER						\
	{ NULL }

#define CLICK_CLICKITYCLACK_RB_INIT(root) do {						\
	(root)->rbh_root = NULL;					\
} while (/*CONSTCOND*/ 0)

#define CLICK_CLICKITYCLACK_RB_BLACK	0
#define CLICK_CLICKITYCLACK_RB_RED		1
#define CLICK_CLICKITYCLACK_RB_ENTRY(type)							\
struct {								\
	type *rbe_left;		/* left element */		\
	type *rbe_right;		/* right element */		\
	type *rbe_parent;	/* parent element */		\
	int rbe_color;			/* node color */		\
}

#define CLICK_CLICKITYCLACK_RB_LEFT(elm, field)		((elm)->*field).rbe_left
#define CLICK_CLICKITYCLACK_RB_RIGHT(elm, field)		((elm)->*field).rbe_right
#define CLICK_CLICKITYCLACK_RB_PARENT(elm, field)		((elm)->*field).rbe_parent
#define CLICK_CLICKITYCLACK_RB_COLOR(elm, field)		((elm)->*field).rbe_color
#define CLICK_CLICKITYCLACK_RB_ROOT(head)			(head)->rbh_root
#define CLICK_CLICKITYCLACK_RB_EMPTY(head)			(CLICK_CLICKITYCLACK_RB_ROOT(head) == NULL)

#define CLICK_CLICKITYCLACK_RB_SET(elm, parent, field) do {					\
	CLICK_CLICKITYCLACK_RB_PARENT(elm, field) = parent;					\
	CLICK_CLICKITYCLACK_RB_LEFT(elm, field) = CLICK_CLICKITYCLACK_RB_RIGHT(elm, field) = NULL;		\
	CLICK_CLICKITYCLACK_RB_COLOR(elm, field) = CLICK_CLICKITYCLACK_RB_RED;					\
} while (/*CONSTCOND*/ 0)

#define CLICK_CLICKITYCLACK_RB_SET_BLACKRED(black, red, field) do {				\
	CLICK_CLICKITYCLACK_RB_COLOR(black, field) = CLICK_CLICKITYCLACK_RB_BLACK;				\
	CLICK_CLICKITYCLACK_RB_COLOR(red, field) = CLICK_CLICKITYCLACK_RB_RED;					\
} while (/*CONSTCOND*/ 0)

#ifndef CLICK_CLICKITYCLACK_RB_AUGMENT
#define CLICK_CLICKITYCLACK_RB_AUGMENT(x)	do {} while (0)
#endif

#define CLICK_CLICKITYCLACK_RB_ROTATE_LEFT(head, elm, tmp, field) do {			\
	(tmp) = CLICK_CLICKITYCLACK_RB_RIGHT(elm, field);					\
	if ((CLICK_CLICKITYCLACK_RB_RIGHT(elm, field) = CLICK_CLICKITYCLACK_RB_LEFT(tmp, field)) != NULL) {	\
		CLICK_CLICKITYCLACK_RB_PARENT(CLICK_CLICKITYCLACK_RB_LEFT(tmp, field), field) = (elm);		\
	}								\
	CLICK_CLICKITYCLACK_RB_AUGMENT(elm);						\
	if ((CLICK_CLICKITYCLACK_RB_PARENT(tmp, field) = CLICK_CLICKITYCLACK_RB_PARENT(elm, field)) != NULL) {	\
		if ((elm) == CLICK_CLICKITYCLACK_RB_LEFT(CLICK_CLICKITYCLACK_RB_PARENT(elm, field), field))	\
			CLICK_CLICKITYCLACK_RB_LEFT(CLICK_CLICKITYCLACK_RB_PARENT(elm, field), field) = (tmp);	\
		else							\
			CLICK_CLICKITYCLACK_RB_RIGHT(CLICK_CLICKITYCLACK_RB_PARENT(elm, field), field) = (tmp);	\
	} else								\
		(head)->rbh_root = (tmp);				\
	CLICK_CLICKITYCLACK_RB_LEFT(tmp, field) = (elm);					\
	CLICK_CLICKITYCLACK_RB_PARENT(elm, field) = (tmp);					\
	CLICK_CLICKITYCLACK_RB_AUGMENT(tmp);						\
	if ((CLICK_CLICKITYCLACK_RB_PARENT(tmp, field)))					\
		CLICK_CLICKITYCLACK_RB_AUGMENT(CLICK_CLICKITYCLACK_RB_PARENT(tmp, field));			\
} while (/*CONSTCOND*/ 0)

#define CLICK_CLICKITYCLACK_RB_ROTATE_RIGHT(head, elm, tmp, field) do {			\
	(tmp) = CLICK_CLICKITYCLACK_RB_LEFT(elm, field);					\
	if ((CLICK_CLICKITYCLACK_RB_LEFT(elm, field) = CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field)) != NULL) {	\
		CLICK_CLICKITYCLACK_RB_PARENT(CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field), field) = (elm);		\
	}								\
	CLICK_CLICKITYCLACK_RB_AUGMENT(elm);						\
	if ((CLICK_CLICKITYCLACK_RB_PARENT(tmp, field) = CLICK_CLICKITYCLACK_RB_PARENT(elm, field)) != NULL) {	\
		if ((elm) == CLICK_CLICKITYCLACK_RB_LEFT(CLICK_CLICKITYCLACK_RB_PARENT(elm, field), field))	\
			CLICK_CLICKITYCLACK_RB_LEFT(CLICK_CLICKITYCLACK_RB_PARENT(elm, field), field) = (tmp);	\
		else							\
			CLICK_CLICKITYCLACK_RB_RIGHT(CLICK_CLICKITYCLACK_RB_PARENT(elm, field), field) = (tmp);	\
	} else								\
		(head)->rbh_root = (tmp);				\
	CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field) = (elm);					\
	CLICK_CLICKITYCLACK_RB_PARENT(elm, field) = (tmp);					\
	CLICK_CLICKITYCLACK_RB_AUGMENT(tmp);						\
	if ((CLICK_CLICKITYCLACK_RB_PARENT(tmp, field)))					\
		CLICK_CLICKITYCLACK_RB_AUGMENT(CLICK_CLICKITYCLACK_RB_PARENT(tmp, field));			\
} while (/*CONSTCOND*/ 0)

/* Generates prototypes and inline functions */
#define	CLICK_CLICKITYCLACK_RB_PROTOTYPE(name, type, field, cmp)				\
	CLICK_CLICKITYCLACK_RB_PROTOTYPE_INTERNAL(name, type, field, cmp,)
#define	CLICK_CLICKITYCLACK_RB_PROTOTYPE_STATIC(name, type, field, cmp)			\
	CLICK_CLICKITYCLACK_RB_PROTOTYPE_INTERNAL(name, type, field, cmp, __attribute__((__unused__)) static)
#define CLICK_CLICKITYCLACK_RB_PROTOTYPE_INTERNAL(name, type, field, cmp, attr)		\
attr void name##_CLICK_CLICKITYCLACK_RB_INSERT_COLOR(struct name *, type *);		\
attr void name##_CLICK_CLICKITYCLACK_RB_REMOVE_COLOR(struct name *, type *, type *);\
attr type *name##_CLICK_CLICKITYCLACK_RB_REMOVE(struct name *, type *);	\
attr type *name##_CLICK_CLICKITYCLACK_RB_INSERT(struct name *, type *);	\
attr type *name##_CLICK_CLICKITYCLACK_RB_FIND(struct name *, type *);		\
attr type *name##_CLICK_CLICKITYCLACK_RB_NFIND(struct name *, type *);	\
attr type *name##_CLICK_CLICKITYCLACK_RB_NEXT(type *);			\
attr type *name##_CLICK_CLICKITYCLACK_RB_PREV(type *);			\
attr type *name##_CLICK_CLICKITYCLACK_RB_MINMAX(struct name *, int);			\
									\

/* Main rb operation.
 * Moves node close to the key of elm to top
 */
#define	CLICK_CLICKITYCLACK_RB_GENERATE(name, type, field, cmp)				\
	CLICK_CLICKITYCLACK_RB_GENERATE_INTERNAL(name, type, field, cmp,)
#define	CLICK_CLICKITYCLACK_RB_GENERATE_STATIC(name, type, field, cmp)			\
	CLICK_CLICKITYCLACK_RB_GENERATE_INTERNAL(name, type, field, cmp, __attribute__((__unused__)) static)
#define CLICK_CLICKITYCLACK_RB_GENERATE_INTERNAL(name, type, field, cmp, attr)		\
attr void								\
name##_CLICK_CLICKITYCLACK_RB_INSERT_COLOR(struct name *head, type *elm)		\
{									\
	type *parent, *gparent, *tmp;				\
	while ((parent = CLICK_CLICKITYCLACK_RB_PARENT(elm, field)) != NULL &&		\
	    CLICK_CLICKITYCLACK_RB_COLOR(parent, field) == CLICK_CLICKITYCLACK_RB_RED) {			\
		gparent = CLICK_CLICKITYCLACK_RB_PARENT(parent, field);			\
		if (parent == CLICK_CLICKITYCLACK_RB_LEFT(gparent, field)) {		\
			tmp = CLICK_CLICKITYCLACK_RB_RIGHT(gparent, field);			\
			if (tmp && CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) == CLICK_CLICKITYCLACK_RB_RED) {	\
				CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) = CLICK_CLICKITYCLACK_RB_BLACK;	\
				CLICK_CLICKITYCLACK_RB_SET_BLACKRED(parent, gparent, field);\
				elm = gparent;				\
				continue;				\
			}						\
			if (CLICK_CLICKITYCLACK_RB_RIGHT(parent, field) == elm) {		\
				CLICK_CLICKITYCLACK_RB_ROTATE_LEFT(head, parent, tmp, field);\
				tmp = parent;				\
				parent = elm;				\
				elm = tmp;				\
			}						\
			CLICK_CLICKITYCLACK_RB_SET_BLACKRED(parent, gparent, field);	\
			CLICK_CLICKITYCLACK_RB_ROTATE_RIGHT(head, gparent, tmp, field);	\
		} else {						\
			tmp = CLICK_CLICKITYCLACK_RB_LEFT(gparent, field);			\
			if (tmp && CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) == CLICK_CLICKITYCLACK_RB_RED) {	\
				CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) = CLICK_CLICKITYCLACK_RB_BLACK;	\
				CLICK_CLICKITYCLACK_RB_SET_BLACKRED(parent, gparent, field);\
				elm = gparent;				\
				continue;				\
			}						\
			if (CLICK_CLICKITYCLACK_RB_LEFT(parent, field) == elm) {		\
				CLICK_CLICKITYCLACK_RB_ROTATE_RIGHT(head, parent, tmp, field);\
				tmp = parent;				\
				parent = elm;				\
				elm = tmp;				\
			}						\
			CLICK_CLICKITYCLACK_RB_SET_BLACKRED(parent, gparent, field);	\
			CLICK_CLICKITYCLACK_RB_ROTATE_LEFT(head, gparent, tmp, field);	\
		}							\
	}								\
	CLICK_CLICKITYCLACK_RB_COLOR(head->rbh_root, field) = CLICK_CLICKITYCLACK_RB_BLACK;			\
}									\
									\
attr void								\
name##_CLICK_CLICKITYCLACK_RB_REMOVE_COLOR(struct name *head, type *parent, type *elm) \
{									\
	type *tmp;						\
	while ((elm == NULL || CLICK_CLICKITYCLACK_RB_COLOR(elm, field) == CLICK_CLICKITYCLACK_RB_BLACK) &&	\
	    elm != CLICK_CLICKITYCLACK_RB_ROOT(head)) {					\
		if (CLICK_CLICKITYCLACK_RB_LEFT(parent, field) == elm) {			\
			tmp = CLICK_CLICKITYCLACK_RB_RIGHT(parent, field);			\
			if (CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) == CLICK_CLICKITYCLACK_RB_RED) {		\
				CLICK_CLICKITYCLACK_RB_SET_BLACKRED(tmp, parent, field);	\
				CLICK_CLICKITYCLACK_RB_ROTATE_LEFT(head, parent, tmp, field);\
				tmp = CLICK_CLICKITYCLACK_RB_RIGHT(parent, field);		\
			}						\
			if ((CLICK_CLICKITYCLACK_RB_LEFT(tmp, field) == NULL ||		\
			    CLICK_CLICKITYCLACK_RB_COLOR(CLICK_CLICKITYCLACK_RB_LEFT(tmp, field), field) == CLICK_CLICKITYCLACK_RB_BLACK) &&\
			    (CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field) == NULL ||		\
			    CLICK_CLICKITYCLACK_RB_COLOR(CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field), field) == CLICK_CLICKITYCLACK_RB_BLACK)) {\
				CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) = CLICK_CLICKITYCLACK_RB_RED;		\
				elm = parent;				\
				parent = CLICK_CLICKITYCLACK_RB_PARENT(elm, field);		\
			} else {					\
				if (CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field) == NULL ||	\
				    CLICK_CLICKITYCLACK_RB_COLOR(CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field), field) == CLICK_CLICKITYCLACK_RB_BLACK) {\
					type *oleft;		\
					if ((oleft = CLICK_CLICKITYCLACK_RB_LEFT(tmp, field)) \
					    != NULL)			\
						CLICK_CLICKITYCLACK_RB_COLOR(oleft, field) = CLICK_CLICKITYCLACK_RB_BLACK;\
					CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) = CLICK_CLICKITYCLACK_RB_RED;	\
					CLICK_CLICKITYCLACK_RB_ROTATE_RIGHT(head, tmp, oleft, field);\
					tmp = CLICK_CLICKITYCLACK_RB_RIGHT(parent, field);	\
				}					\
				CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) = CLICK_CLICKITYCLACK_RB_COLOR(parent, field);\
				CLICK_CLICKITYCLACK_RB_COLOR(parent, field) = CLICK_CLICKITYCLACK_RB_BLACK;	\
				if (CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field))		\
					CLICK_CLICKITYCLACK_RB_COLOR(CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field), field) = CLICK_CLICKITYCLACK_RB_BLACK;\
				CLICK_CLICKITYCLACK_RB_ROTATE_LEFT(head, parent, tmp, field);\
				elm = CLICK_CLICKITYCLACK_RB_ROOT(head);			\
				break;					\
			}						\
		} else {						\
			tmp = CLICK_CLICKITYCLACK_RB_LEFT(parent, field);			\
			if (CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) == CLICK_CLICKITYCLACK_RB_RED) {		\
				CLICK_CLICKITYCLACK_RB_SET_BLACKRED(tmp, parent, field);	\
				CLICK_CLICKITYCLACK_RB_ROTATE_RIGHT(head, parent, tmp, field);\
				tmp = CLICK_CLICKITYCLACK_RB_LEFT(parent, field);		\
			}						\
			if ((CLICK_CLICKITYCLACK_RB_LEFT(tmp, field) == NULL ||		\
			    CLICK_CLICKITYCLACK_RB_COLOR(CLICK_CLICKITYCLACK_RB_LEFT(tmp, field), field) == CLICK_CLICKITYCLACK_RB_BLACK) &&\
			    (CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field) == NULL ||		\
			    CLICK_CLICKITYCLACK_RB_COLOR(CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field), field) == CLICK_CLICKITYCLACK_RB_BLACK)) {\
				CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) = CLICK_CLICKITYCLACK_RB_RED;		\
				elm = parent;				\
				parent = CLICK_CLICKITYCLACK_RB_PARENT(elm, field);		\
			} else {					\
				if (CLICK_CLICKITYCLACK_RB_LEFT(tmp, field) == NULL ||	\
				    CLICK_CLICKITYCLACK_RB_COLOR(CLICK_CLICKITYCLACK_RB_LEFT(tmp, field), field) == CLICK_CLICKITYCLACK_RB_BLACK) {\
					type *oright;		\
					if ((oright = CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field)) \
					    != NULL)			\
						CLICK_CLICKITYCLACK_RB_COLOR(oright, field) = CLICK_CLICKITYCLACK_RB_BLACK;\
					CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) = CLICK_CLICKITYCLACK_RB_RED;	\
					CLICK_CLICKITYCLACK_RB_ROTATE_LEFT(head, tmp, oright, field);\
					tmp = CLICK_CLICKITYCLACK_RB_LEFT(parent, field);	\
				}					\
				CLICK_CLICKITYCLACK_RB_COLOR(tmp, field) = CLICK_CLICKITYCLACK_RB_COLOR(parent, field);\
				CLICK_CLICKITYCLACK_RB_COLOR(parent, field) = CLICK_CLICKITYCLACK_RB_BLACK;	\
				if (CLICK_CLICKITYCLACK_RB_LEFT(tmp, field))		\
					CLICK_CLICKITYCLACK_RB_COLOR(CLICK_CLICKITYCLACK_RB_LEFT(tmp, field), field) = CLICK_CLICKITYCLACK_RB_BLACK;\
				CLICK_CLICKITYCLACK_RB_ROTATE_RIGHT(head, parent, tmp, field);\
				elm = CLICK_CLICKITYCLACK_RB_ROOT(head);			\
				break;					\
			}						\
		}							\
	}								\
	if (elm)							\
		CLICK_CLICKITYCLACK_RB_COLOR(elm, field) = CLICK_CLICKITYCLACK_RB_BLACK;			\
}									\
									\
attr type *							\
name##_CLICK_CLICKITYCLACK_RB_REMOVE(struct name *head, type *elm)			\
{									\
	type *child, *parent, *old = elm;			\
	int color;							\
	if (CLICK_CLICKITYCLACK_RB_LEFT(elm, field) == NULL)				\
		child = CLICK_CLICKITYCLACK_RB_RIGHT(elm, field);				\
	else if (CLICK_CLICKITYCLACK_RB_RIGHT(elm, field) == NULL)				\
		child = CLICK_CLICKITYCLACK_RB_LEFT(elm, field);				\
	else {								\
		type *left;					\
		elm = CLICK_CLICKITYCLACK_RB_RIGHT(elm, field);				\
		while ((left = CLICK_CLICKITYCLACK_RB_LEFT(elm, field)) != NULL)		\
			elm = left;					\
		child = CLICK_CLICKITYCLACK_RB_RIGHT(elm, field);				\
		parent = CLICK_CLICKITYCLACK_RB_PARENT(elm, field);				\
		color = CLICK_CLICKITYCLACK_RB_COLOR(elm, field);				\
		if (child)						\
			CLICK_CLICKITYCLACK_RB_PARENT(child, field) = parent;		\
		if (parent) {						\
			if (CLICK_CLICKITYCLACK_RB_LEFT(parent, field) == elm)		\
				CLICK_CLICKITYCLACK_RB_LEFT(parent, field) = child;		\
			else						\
				CLICK_CLICKITYCLACK_RB_RIGHT(parent, field) = child;	\
			CLICK_CLICKITYCLACK_RB_AUGMENT(parent);				\
		} else							\
			CLICK_CLICKITYCLACK_RB_ROOT(head) = child;				\
		if (CLICK_CLICKITYCLACK_RB_PARENT(elm, field) == old)			\
			parent = elm;					\
		(elm)->*field = (old)->*field;				\
		if (CLICK_CLICKITYCLACK_RB_PARENT(old, field)) {				\
			if (CLICK_CLICKITYCLACK_RB_LEFT(CLICK_CLICKITYCLACK_RB_PARENT(old, field), field) == old)\
				CLICK_CLICKITYCLACK_RB_LEFT(CLICK_CLICKITYCLACK_RB_PARENT(old, field), field) = elm;\
			else						\
				CLICK_CLICKITYCLACK_RB_RIGHT(CLICK_CLICKITYCLACK_RB_PARENT(old, field), field) = elm;\
			CLICK_CLICKITYCLACK_RB_AUGMENT(CLICK_CLICKITYCLACK_RB_PARENT(old, field));		\
		} else							\
			CLICK_CLICKITYCLACK_RB_ROOT(head) = elm;				\
		CLICK_CLICKITYCLACK_RB_PARENT(CLICK_CLICKITYCLACK_RB_LEFT(old, field), field) = elm;		\
		if (CLICK_CLICKITYCLACK_RB_RIGHT(old, field))				\
			CLICK_CLICKITYCLACK_RB_PARENT(CLICK_CLICKITYCLACK_RB_RIGHT(old, field), field) = elm;	\
		if (parent) {						\
			left = parent;					\
			do {						\
				CLICK_CLICKITYCLACK_RB_AUGMENT(left);			\
			} while ((left = CLICK_CLICKITYCLACK_RB_PARENT(left, field)) != NULL); \
		}							\
		goto color;						\
	}								\
	parent = CLICK_CLICKITYCLACK_RB_PARENT(elm, field);					\
	color = CLICK_CLICKITYCLACK_RB_COLOR(elm, field);					\
	if (child)							\
		CLICK_CLICKITYCLACK_RB_PARENT(child, field) = parent;			\
	if (parent) {							\
		if (CLICK_CLICKITYCLACK_RB_LEFT(parent, field) == elm)			\
			CLICK_CLICKITYCLACK_RB_LEFT(parent, field) = child;			\
		else							\
			CLICK_CLICKITYCLACK_RB_RIGHT(parent, field) = child;		\
		CLICK_CLICKITYCLACK_RB_AUGMENT(parent);					\
	} else								\
		CLICK_CLICKITYCLACK_RB_ROOT(head) = child;					\
color:									\
	if (color == CLICK_CLICKITYCLACK_RB_BLACK)						\
		name##_CLICK_CLICKITYCLACK_RB_REMOVE_COLOR(head, parent, child);		\
	return (old);							\
}									\
									\
/* Inserts a node into the RB tree */					\
attr type *							\
name##_CLICK_CLICKITYCLACK_RB_INSERT(struct name *head, type *elm)			\
{									\
	type *tmp;						\
	type *parent = NULL;					\
	int comp = 0;							\
	tmp = CLICK_CLICKITYCLACK_RB_ROOT(head);						\
	while (tmp) {							\
		parent = tmp;						\
		comp = (cmp)(elm, parent);				\
		if (comp < 0)						\
			tmp = CLICK_CLICKITYCLACK_RB_LEFT(tmp, field);			\
		else if (comp > 0)					\
			tmp = CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field);			\
		else							\
			return (tmp);					\
	}								\
	CLICK_CLICKITYCLACK_RB_SET(elm, parent, field);					\
	if (parent != NULL) {						\
		if (comp < 0)						\
			CLICK_CLICKITYCLACK_RB_LEFT(parent, field) = elm;			\
		else							\
			CLICK_CLICKITYCLACK_RB_RIGHT(parent, field) = elm;			\
		CLICK_CLICKITYCLACK_RB_AUGMENT(parent);					\
	} else								\
		CLICK_CLICKITYCLACK_RB_ROOT(head) = elm;					\
	name##_CLICK_CLICKITYCLACK_RB_INSERT_COLOR(head, elm);				\
	return (NULL);							\
}									\
									\
/* Finds the node with the same key as elm */				\
attr type *							\
name##_CLICK_CLICKITYCLACK_RB_FIND(struct name *head, type *elm)			\
{									\
	type *tmp = CLICK_CLICKITYCLACK_RB_ROOT(head);				\
	int comp;							\
	while (tmp) {							\
		comp = cmp(elm, tmp);					\
		if (comp < 0)						\
			tmp = CLICK_CLICKITYCLACK_RB_LEFT(tmp, field);			\
		else if (comp > 0)					\
			tmp = CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field);			\
		else							\
			return (tmp);					\
	}								\
	return (NULL);							\
}									\
									\
/* Finds the first node greater than or equal to the search key */	\
attr type *							\
name##_CLICK_CLICKITYCLACK_RB_NFIND(struct name *head, type *elm)			\
{									\
	type *tmp = CLICK_CLICKITYCLACK_RB_ROOT(head);				\
	type *res = NULL;					\
	int comp;							\
	while (tmp) {							\
		comp = cmp(elm, tmp);					\
		if (comp < 0) {						\
			res = tmp;					\
			tmp = CLICK_CLICKITYCLACK_RB_LEFT(tmp, field);			\
		}							\
		else if (comp > 0)					\
			tmp = CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field);			\
		else							\
			return (tmp);					\
	}								\
	return (res);							\
}									\
									\
/* ARGSUSED */								\
attr type *							\
name##_CLICK_CLICKITYCLACK_RB_NEXT(type *elm)					\
{									\
	if (CLICK_CLICKITYCLACK_RB_RIGHT(elm, field)) {					\
		elm = CLICK_CLICKITYCLACK_RB_RIGHT(elm, field);				\
		while (CLICK_CLICKITYCLACK_RB_LEFT(elm, field))				\
			elm = CLICK_CLICKITYCLACK_RB_LEFT(elm, field);			\
	} else {							\
		if (CLICK_CLICKITYCLACK_RB_PARENT(elm, field) &&				\
		    (elm == CLICK_CLICKITYCLACK_RB_LEFT(CLICK_CLICKITYCLACK_RB_PARENT(elm, field), field)))	\
			elm = CLICK_CLICKITYCLACK_RB_PARENT(elm, field);			\
		else {							\
			while (CLICK_CLICKITYCLACK_RB_PARENT(elm, field) &&			\
			    (elm == CLICK_CLICKITYCLACK_RB_RIGHT(CLICK_CLICKITYCLACK_RB_PARENT(elm, field), field)))\
				elm = CLICK_CLICKITYCLACK_RB_PARENT(elm, field);		\
			elm = CLICK_CLICKITYCLACK_RB_PARENT(elm, field);			\
		}							\
	}								\
	return (elm);							\
}									\
									\
/* ARGSUSED */								\
attr type *							\
name##_CLICK_CLICKITYCLACK_RB_PREV(type *elm)					\
{									\
	if (CLICK_CLICKITYCLACK_RB_LEFT(elm, field)) {					\
		elm = CLICK_CLICKITYCLACK_RB_LEFT(elm, field);				\
		while (CLICK_CLICKITYCLACK_RB_RIGHT(elm, field))				\
			elm = CLICK_CLICKITYCLACK_RB_RIGHT(elm, field);			\
	} else {							\
		if (CLICK_CLICKITYCLACK_RB_PARENT(elm, field) &&				\
		    (elm == CLICK_CLICKITYCLACK_RB_RIGHT(CLICK_CLICKITYCLACK_RB_PARENT(elm, field), field)))	\
			elm = CLICK_CLICKITYCLACK_RB_PARENT(elm, field);			\
		else {							\
			while (CLICK_CLICKITYCLACK_RB_PARENT(elm, field) &&			\
			    (elm == CLICK_CLICKITYCLACK_RB_LEFT(CLICK_CLICKITYCLACK_RB_PARENT(elm, field), field)))\
				elm = CLICK_CLICKITYCLACK_RB_PARENT(elm, field);		\
			elm = CLICK_CLICKITYCLACK_RB_PARENT(elm, field);			\
		}							\
	}								\
	return (elm);							\
}									\
									\
attr type *							\
name##_CLICK_CLICKITYCLACK_RB_MINMAX(struct name *head, int val)				\
{									\
	type *tmp = CLICK_CLICKITYCLACK_RB_ROOT(head);				\
	type *parent = NULL;					\
	while (tmp) {							\
		parent = tmp;						\
		if (val < 0)						\
			tmp = CLICK_CLICKITYCLACK_RB_LEFT(tmp, field);			\
		else							\
			tmp = CLICK_CLICKITYCLACK_RB_RIGHT(tmp, field);			\
	}								\
	return (parent);						\
}

#define CLICK_CLICKITYCLACK_RB_NEGINF	-1
#define CLICK_CLICKITYCLACK_RB_INF	1

#define CLICK_CLICKITYCLACK_RB_INSERT(name, x, y)	name##_CLICK_CLICKITYCLACK_RB_INSERT(x, y)
#define CLICK_CLICKITYCLACK_RB_REMOVE(name, x, y)	name##_CLICK_CLICKITYCLACK_RB_REMOVE(x, y)
#define CLICK_CLICKITYCLACK_RB_FIND(name, x, y)	name##_CLICK_CLICKITYCLACK_RB_FIND(x, y)
#define CLICK_CLICKITYCLACK_RB_NFIND(name, x, y)	name##_CLICK_CLICKITYCLACK_RB_NFIND(x, y)
#define CLICK_CLICKITYCLACK_RB_NEXT(name, x, y)	name##_CLICK_CLICKITYCLACK_RB_NEXT(y)
#define CLICK_CLICKITYCLACK_RB_PREV(name, x, y)	name##_CLICK_CLICKITYCLACK_RB_PREV(y)
#define CLICK_CLICKITYCLACK_RB_MIN(name, x)		name##_CLICK_CLICKITYCLACK_RB_MINMAX(x, CLICK_CLICKITYCLACK_RB_NEGINF)
#define CLICK_CLICKITYCLACK_RB_MAX(name, x)		name##_CLICK_CLICKITYCLACK_RB_MINMAX(x, CLICK_CLICKITYCLACK_RB_INF)

#define CLICK_CLICKITYCLACK_RB_FOREACH(x, name, head)					\
	for ((x) = CLICK_CLICKITYCLACK_RB_MIN(name, head);					\
	     (x) != NULL;						\
	     (x) = name##_CLICK_CLICKITYCLACK_RB_NEXT(x))

#define CLICK_CLICKITYCLACK_RB_FOREACH_FROM(x, name, y)					\
	for ((x) = (y);							\
	    ((x) != NULL) && ((y) = name##_CLICK_CLICKITYCLACK_RB_NEXT(x), (x) != NULL);	\
	     (x) = (y))

#define CLICK_CLICKITYCLACK_RB_FOREACH_SAFE(x, name, head, y)				\
	for ((x) = CLICK_CLICKITYCLACK_RB_MIN(name, head);					\
	    ((x) != NULL) && ((y) = name##_CLICK_CLICKITYCLACK_RB_NEXT(x), (x) != NULL);	\
	     (x) = (y))

#define CLICK_CLICKITYCLACK_RB_FOREACH_REVERSE(x, name, head)				\
	for ((x) = CLICK_CLICKITYCLACK_RB_MAX(name, head);					\
	     (x) != NULL;						\
	     (x) = name##_CLICK_CLICKITYCLACK_RB_PREV(x))

#define CLICK_CLICKITYCLACK_RB_FOREACH_REVERSE_FROM(x, name, y)				\
	for ((x) = (y);							\
	    ((x) != NULL) && ((y) = name##_CLICK_CLICKITYCLACK_RB_PREV(x), (x) != NULL);	\
	     (x) = (y))

#define CLICK_CLICKITYCLACK_RB_FOREACH_REVERSE_SAFE(x, name, head, y)			\
	for ((x) = CLICK_CLICKITYCLACK_RB_MAX(name, head);					\
	    ((x) != NULL) && ((y) = name##_CLICK_CLICKITYCLACK_RB_PREV(x), (x) != NULL);	\
	     (x) = (y))

#endif	/* CLICK_CLICKITYCLACK_FREEBSDTREE_H */
