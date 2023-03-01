//**************************************************************
// This module carries out graph duplication. This is done in
// a top-down manner, starting off from the root and going
// downwards. However, when entering a FAN, we have to take
// into account the possibility to have already passed through
// it. Therefore, we have to keep track of the relations
// existing between source and destination FANS.
// The same issue holds true for all those forms that "contain"
// a FAN, such as CAR1, CDR and TESTNIL, as well as for LAMBDA.
// In fact, when duplicationg a LAMBDA, we enter from its main
// port and start copying from port 1. Sooner or later, taking
// a certain path, we'll get to port 2, and need to know at
// this stage which is its twin-form.
// In order to maintain the relations between source and
// destination forms, a hash table is used. This table is
// accessed by the source form's pointer.
// The following function is extern:
//  - copy(): Initialises the hash table, calls function
//            copy_aux and eliminates the table.
// The following functions are internal:
//  - copy_aux(): Duplicates the input graph and returns it as
//	          output.
//  - put_relation(): Inserts a two-form relation in the table
//  - is_in_relation(): Checks whether or not a form has already
//                      been copied.
//  - entry(): Implements hash function.
//  - start_copy(): Initialises hash table.
//  - end_copy(): Eliminates hash table.
//**************************************************************

#include "bohm.h"

#define DIM_REL 256

// static CopyForm *copy_relation[DIM_REL];

Form *copy_connect(
	Form *temp,
	int port,
	Form *newf1,
	int offset)
{
	Form *ret;
	if (is_int(temp->nport[port]))
	{
		ret = temp->nform[port]->copy_aux(temp->nport[port], offset);
		connect(newf1, port, ret, temp->nport[port]);
	}
	else
	{
		int_connect(newf1, port, temp->nform[port], temp->nport[port]);
	}

	return ret;
}

Form *copy_connect1(
	Form *this_,
	Form *newf1,
	int offset)
{
	Form *ret;
	if (is_int(this_->nport[0]))
	{
		ret = this_->nform[0]->copy_aux(this_->nport[0], offset);
		connect1(newf1, 0, ret, this_->nport[0]);
	}
	else
	{
		int_connect(newf1, 0, this_->nform[0], this_->nport[0]);
	}

	return ret;
}

struct Relations final
{
	CopyForm *copy_relation[DIM_REL];

	Relations()
	{
		reset();
	}

	void reset()
	{
		for (int i = 0; i < DIM_REL; i++)
		{
			this->copy_relation[i] = NULL;
		}
	}

	// The following function initialises hash table.
	void start()
	{
		reset();
		//
	}

	// The following function eliminates hash table.
	void end()
	{

		for (int i = 0; i < DIM_REL; i++)
		{
			CopyForm *dep;
			while ((dep = copy_relation[i]) != NULL)
			{
				copy_relation[i] = dep->next;
				free(dep);
			}
		}
	}

	// The following function inserts a two-form relation in the table.
	// put_relation(
	void store(Form *src, Form *dest)
	{
		int dep1 = src->hash();
		CopyForm *dep = new CopyForm(src, dest, this->copy_relation[dep1]);

		this->copy_relation[dep1] = dep;
	}

	// The following function checks whether or not a form has
	// already been copied.
	// is_in_relation
	Form *rel(Form *src)
	{
		CopyForm *dep = this->copy_relation[src->hash()];
		if (dep == NULL)
		{
			return NULL;
		}

		while (dep->src != src && dep->next != NULL)
		{
			dep = dep->next;
		}

		if (dep->src == src)
		{
			return dep->dest;
		}
		return NULL;
		//
	}
};

inline Relations rel = Relations();

// The following function initialises the hash table, calls
// function copy_aux and eliminates the table.
Form *Form::copy(
	int p,
	int offset)
{
	Form *risul;
	rel.start();
	if (p < 0)
	{
		risul = this;
	}
	else
	{
		risul = this->copy_aux(p, offset);
	}
	rel.end();
	return risul;
}

// The following function duplicates the input graph and
// returns it as output.
Form *Form::copy_aux(int p, int offset)
{
	Form *newf1;
	Form *newf2;
	Form *newf3;
	int q;

	switch (this->kind)
	{
	case TRIANGLE:
		newf1 = new Form(this->kind, this->index + offset);
		newf1->nlevel[1] = this->nlevel[1];
		newf2 = copy_connect1(this, newf1, offset);
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
		{
			q = 1;
		}
		else
		{
			q = 0;
		}
		newf1 = new Form(this->kind, this->index + offset);
		newf1->num_safe = this->num_safe;
		newf1->nform[2] = this->nform[2];
		newf2 = copy_connect(this, q, newf1, offset);

		return newf1;

	case LAMBDA:
		if (p != 0)
		{
			return rel.rel(this);
		}
		newf1 = new Form(this->kind, this->index + offset);
		rel.store(this, newf1);
		newf2 = this->nform[1]->copy_aux(this->nport[1], offset);
		::connect1(newf1, 1, newf2, this->nport[1]);
		return newf1;

	case TESTNIL1:
	case CDR1:
	case CAR1:
	case CONS1:
	case FAN:
		if ((newf1 = rel.rel(this)) != NULL)
		{
			return newf1;
		}
		newf1 = new Form(this->kind, this->index + offset);
		newf1->nlevel[1] = this->nlevel[1];
		newf1->nlevel[2] = this->nlevel[2];
		rel.store(this, newf1);

		newf2 = copy_connect1(this, newf1, offset);
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
		newf1 = new Form(this->kind, this->index + offset);
		newf2 = copy_connect(this, 0, newf1, offset);
		newf3 = copy_connect(this, 2, newf1, offset);
		return newf1;

	case CONS:
		newf1 = new Form(this->kind, this->index + offset);
		newf2 = copy_connect(this, 1, newf1, offset);
		newf3 = copy_connect(this, 2, newf1, offset);
		return newf1;

	default:
		return NULL;
	}
}

// The following function inserts a two-form relation in the table.
// static void put_relation(
// 	Form *src,
// 	Form *dest)
// {
// 	int dep1 = src->hash();
// 	CopyForm *dep = new CopyForm(src, dest, copy_relation[dep1]);

// 	copy_relation[dep1] = dep;
// }

// static Form *is_in_relation(Form *src)
// {
// 	CopyForm *dep = copy_relation[src->hash()];
// 	if (dep == NULL)
// 		return NULL;

// 	while (dep->src != src && dep->next != NULL)
// 		dep = dep->next;

// 	if (dep->src == src)
// 		return dep->dest;
// 	return NULL;
// }

// The following function implements hash function.
int Form::hash()
{
	unsigned long risul = (unsigned long)this;
	risul = risul / 8 * 13;
	return risul % DIM_REL;
}
