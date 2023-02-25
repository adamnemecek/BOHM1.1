/****************************************************************/
/* This module carries out graph duplication. This is done in   */
/* a top-down manner, starting off from the root and going      */
/* downwards. However, when entering a FAN, we have to take     */
/* into account the possibility to have already passed through  */
/* it. Therefore, we have to keep track of the relations        */
/* existing between source and destination FANS.		*/
/* The same issue holds true for all those forms that "contain" */
/* a FAN, such as CAR1, CDR and TESTNIL, as well as for LAMBDA. */
/* In fact, when duplicationg a LAMBDA, we enter from its main  */
/* port and start copying from port 1. Sooner or later, taking  */
/* a certain path, we'll get to port 2, and need to know at     */
/* this stage which is its twin-form.				*/
/* In order to maintain the relations between source and        */
/* destination forms, a hash table is used. This table is       */
/* accessed by the source form's pointer.  			*/
/* The following function is extern:				*/
/*  - copy(): Initialises the hash table, calls function        */
/*            copy_aux and eliminates the table.		*/
/* The following functions are internal:			*/
/*  - copy_aux(): Duplicates the input graph and returns it as  */
/*	          output.					*/
/*  - put_relation(): Inserts a two-form relation in the table  */
/*  - is_in_relation(): Checks whether or not a form has already*/
/*                      been copied.                            */
/*  - entry(): Implements hash function.			*/
/*  - start_copy(): Initialises hash table.		        */
/*  - end_copy(): Eliminates hash table.			*/
/****************************************************************/

#include "bohm.h"

#include <stdio.h>
#include <stdlib.h>

#define DIM_REL 256

static COPY_FORM *copy_relation[DIM_REL];
static void put_relation(
	FORM *src,
	FORM *dest);
static void start_copy(void);
static void end_copy(void);
static FORM *is_in_relation(FORM *src);
static FORM *copy_aux(
	FORM *root,
	int p,
	int offset);
static int entry(FORM *src);

/* The following function initialises the hash table, calls 	*/
/* function copy_aux and eliminates the table.			*/
FORM *copy(
	FORM *root,
	int p,
	int offset)
{
	FORM *risul;
	start_copy();
	if (p < 0)
		risul = root;
	else
		risul = copy_aux(root, p, offset);
	end_copy();
	return risul;
}

/* The following function duplicates the input graph and 	*/
/* returns it as output.					*/
static FORM *copy_aux(
	FORM *root,
	int p,
	int offset)
{
	FORM *newf1,
		*newf2,
		*newf3;
	int q;

	FORM *temp = root;
	switch (temp->kind)
	{
	case TRIANGLE:
		newf1 = allocate_form(temp->kind, (temp->index) + offset);
		newf1->nlevel[1] = temp->nlevel[1];
		if (temp->nport[0] >= 0)
		{
			newf2 = copy_aux(temp->nform[0], temp->nport[0], offset);
			connect1(newf1, 0, newf2, temp->nport[0]);
		}
		else
			int_connect(newf1, 0, temp->nform[0], temp->nport[0]);
		return newf1;

	case NOTEQ1:
	case ADD1:
	case SUB1:
	case PROD1:
	case DIV1:
	case MOD1:
	case LESS1:
	case EQ1:
	case MORE1:
	case LEQ1:
	case MEQ1:
	case NOT:
	case CAR:
	case CDR:
	case TESTNIL:
	case LAMBDAUNB:
		if (p == 0)
			q = 1;
		else
			q = 0;
		newf1 = allocate_form(temp->kind, (temp->index) + offset);
		newf1->num_safe = temp->num_safe;
		newf1->nform[2] = temp->nform[2];
		if (temp->nport[q] >= 0)
		{
			newf2 = copy_aux(temp->nform[q], temp->nport[q], offset);
			connect(newf1, q, newf2, temp->nport[q]);
		}
		else
			int_connect(newf1, q, temp->nform[q], temp->nport[q]);
		return newf1;

	case LAMBDA:
		if (p != 0)
			return is_in_relation(temp);
		newf1 = allocate_form(temp->kind, (temp->index) + offset);
		put_relation(temp, newf1);
		newf2 = copy_aux(temp->nform[1], temp->nport[1], offset);
		connect1(newf1, 1, newf2, temp->nport[1]);
		return newf1;

	case TESTNIL1:
	case CDR1:
	case CAR1:
	case CONS1:
	case FAN:
		if ((newf1 = is_in_relation(temp)) != NULL)
			return newf1;
		newf1 = allocate_form(temp->kind, (temp->index) + offset);
		newf1->nlevel[1] = temp->nlevel[1];
		newf1->nlevel[2] = temp->nlevel[2];
		put_relation(temp, newf1);
		if (temp->nport[0] >= 0)
		{
			newf2 = copy_aux(temp->nform[0], temp->nport[0], offset);
			connect1(newf1, 0, newf2, temp->nport[0]);
		}
		else
			int_connect(newf1, 0, temp->nform[0], temp->nport[0]);
		return newf1;

	case APP:
	case AND:
	case OR:
	case ADD:
	case SUB:
	case PROD:
	case DIV:
	case MOD:
	case LESS:
	case EQ:
	case NOTEQ:
	case MORE:
	case LEQ:
	case MEQ:
	case IFELSE:
		newf1 = allocate_form(temp->kind, (temp->index) + offset);
		if (temp->nport[0] >= 0)
		{
			newf2 = copy_aux(temp->nform[0], temp->nport[0], offset);
			connect(newf1, 0, newf2, temp->nport[0]);
		}
		else
			int_connect(newf1, 0, temp->nform[0], temp->nport[0]);
		if (temp->nport[2] >= 0)
		{
			newf3 = copy_aux(temp->nform[2], temp->nport[2], offset);
			connect(newf1, 2, newf3, temp->nport[2]);
		}
		else
			int_connect(newf1, 2, temp->nform[2], temp->nport[2]);
		return newf1;

	case CONS:
		newf1 = allocate_form(temp->kind, (temp->index) + offset);
		if (temp->nport[1] >= 0)
		{
			newf2 = copy_aux(temp->nform[1], temp->nport[1], offset);
			connect(newf1, 1, newf2, temp->nport[1]);
		}
		else
			int_connect(newf1, 1, temp->nform[1], temp->nport[1]);
		if (temp->nport[2] >= 0)
		{
			newf3 = copy_aux(temp->nform[2], temp->nport[2], offset);
			connect(newf1, 2, newf3, temp->nport[2]);
		}
		else
			int_connect(newf1, 2, temp->nform[2], temp->nport[2]);
		return newf1;
	default:
		return NULL;
	}
}

/* The following function inserts a two-form relation in the table. */
static void put_relation(
	FORM *src,
	FORM *dest)
{
	COPY_FORM *dep = (COPY_FORM *)malloc_da(sizeof(COPY_FORM));
	int dep1 = entry(src);
	dep->src = src;
	dep->dest = dest;
	dep->next = copy_relation[dep1];
	copy_relation[dep1] = dep;
}

/* The following function checks whether or not a form has 	*/
/* already been copied.						*/
static FORM *is_in_relation(FORM *src)
{
	COPY_FORM *dep = copy_relation[entry(src)];
	if (dep == NULL)
		return NULL;

	while (dep->src != src && dep->next != NULL)
		dep = dep->next;

	if (dep->src == src)
		return dep->dest;
	return NULL;
}

/* The following function implements hash function.		*/
static int entry(FORM *src)
{
	unsigned long risul = (unsigned long)src;
	risul = risul / 8 * 13;
	return risul % DIM_REL;
}

/* The following function initialises hash table.	        */
static void
start_copy(void)
{
	for (int i = 0; i < DIM_REL; i++)
		copy_relation[i] = NULL;
}

/* The following function eliminates hash table.		*/
static void
end_copy(void)
{
	COPY_FORM *dep;

	for (int i = 0; i < DIM_REL; i++)
	{
		while ((dep = copy_relation[i]) != NULL)
		{
			copy_relation[i] = dep->next;
			free(dep);
		}
	}
}
