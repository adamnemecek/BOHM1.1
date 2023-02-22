/****************************************************************/
/* This module contains routines for generating the graph       */
/* representation of lambda-terms.                              */
/* The translation which has been implemented is the one        */
/* described in "linear Logic, Comonads, and Optimal            */
/* Reductions", by A.Asperti, to appear in Fundamenta           */
/* Informaticae.                                                */
/* During the translation, each term is described by a record   */
/* with following fields:                                       */
/*  - root_form : the root form of the term;                        */
/*  - root_ports : the root (positive) port of the root form;        */
/*  - vars:   a pointer to the list of free variables for the   */
/*            term.                                             */
/* Free variable of a same term are collected into a linked     */
/* list. Each entry for a free variable consists of:            */
/*  - a pointer to the next variable;                           */
/*  - a pointer to the last form associated to the variable.    */
/*  - a pointer to the symbol table bucket associated with the  */
/*    the variable.                                             */
/* The support of the implementation is an interaction net.     */
/* Each connection between forms of the interaction net has     */
/* been implemented by a double pointer. This is probably       */
/* a bit redundant and inefficient, but it greatly simplified   */
/* debugging, as it allows an easy inspection of the resulting  */
/* graph (see also the directive "inspect" of the language).    */
/* The following functions are external                         */
/*  - buildvarterm():  it builds the graph corresponding to a   */
/*                     a variable. If the variable has been     */
/*                     globally defined by a let instruction,   */
/*                     the corresponding graph is a (shared)    */
/*                     instance of the corresponding term.      */
/*  - buildplambdaterm(): it builds the graph corresponding to  */
/*                        a lambda abstraction.                 */
/*  - buildappterm(): it builds the graph corresponding to an   */
/*                    application                               */
/*  - buildletinterm(): it builds the graph corresponding to a  */
/*                      let_in instruction (it is equivalent to */
/*                      a partially evaluated lambda redex)     */
/*  - closeterm(): it adds a root node to a term 		*/
/*  - connect(): it connects two graphical forms at two         */
/*               specified ports.                               */
/*  - connect1():  it connects the port portf1 of form1 to the 	*/
/* 		   port portf2 of form2 and vice versa if the 	*/
/*		   form2 is not a NIL, INT, True or False form.	*/
/*  - int_connect(): it connects only the port portf1 of form1	*/
/*		    to the port portf2 of form2, becouse form2  */
/*		    is a INT.					*/
/*  - bool_connect(): it connects only the port portf1 of form1	*/
/* 		      to the port portf2 of form2, because 	*/
/*		      form2 is a NIL, True or False.		*/
/*  - myfree():  it is used to handle a free list containig     */
/*               all forms removed from the graph.              */
/*		 These forms will be used for future            */
/*		 allocations.                                   */
/*  - allocate_form(): it allocates a new graphical form.       */
/* The functions buildvarterm(), buildplambdaterm(),            */
/* buildappterm() and buildletinterm() etc. are called by the   */
/* parser.                                                      */
/* The functions connect() and allocate_form() are also used    */
/* during reduction.                                            */
/* The following functions are local to the module:             */
/*  - allocate_var(): it allocates a new entry for a variable.  */
/*  - allocate_term(): it allocates a new entry for a term.     */
/*  - makebox(): it builds a box around a term.                 */
/*  - addbrackets(): it adds a sequence of brackets of          */
/*                   specified index along a sequence of        */
/*                   variables.                                 */
/*  - share(): it shares the free variables of two terms, by    */
/*             adding suitable FANS.                            */
/*  - lookfor(): it searches for a variable inside a list.      */
/*  - remv(): it removes a variable from a list.                */
/****************************************************************/

#include "bohm.h"

#include <stdio.h>
#include <stdlib.h>

/* constants concerning allocate form */
#define FORM_NUM 1000

unsigned num_nodes, max_nodes;

unsigned length_list = 0;

static TERM *makebox(int level, TERM *arg);

static VARENTRY *addbrackets(
	int index,
	VARENTRY *listvar);

static VARENTRY *share(
	int index,
	VARENTRY *l1,
	VARENTRY *l2);

static VARENTRY *lookfor(
	STBUCKET *id,
	VARENTRY *listvar);

static VARENTRY *remv(
	STBUCKET *id,
	VARENTRY *listvar);

static VARENTRY *remvp(
	VARLIST *vl,
	VARENTRY *listvar);

static VARENTRY *allocate_var(
	STBUCKET *id,
	FORM *form,
	VARENTRY *nextvar);

static void closeglobalvars(VARENTRY *listvar);
static void intelligent_connect(
	FORM *f1,
	int port,
	FORM *f2);

static void inspect_connect(
	FORM *f1,
	int p1,
	FORM *f2,
	int p2);

TERM *allocate_term(
	FORM *root_form,
	int root_ports,
	VARENTRY *freevars);

/* The following function creates the graph representation of */
/* a variable */
TERM *buildvarterm(int level, STBUCKET *id)
{
	FORM *newf;		  /* pointer to the new form to be created */
	VARENTRY *newvar; /* pointer to the new free variable entry */

	newf = allocate_form(TRIANGLE, level);
	newf->nlevel[1] = -1;
	newvar = allocate_var(id, newf, NULL);
	return allocate_term(newf, 1, newvar);
}

/* The following function creates the graph representation of */
/* a true constant */
TERM *buildtrueterm(int level)
{
	return allocate_term(NULL, T, NULL);
}

/* The following function creates the graph representation of */
/* a false constant */
TERM *buildfalseterm(int level)
{
	/* pointer to the term to be created */
	return allocate_term(NULL, F, NULL);
}

/* The following function creates the graph representation of */
/* a numerical constant */
TERM *buildintterm(int level, long int value)
{
	/* pointer to the term to be created */

	return allocate_term((FORM *)value, INT, NULL);
}

/* The following function creates the graph representation of a */
/* lambda-abstraction */
static TERM *buildlambdaterm(int level, STBUCKET *id, TERM *body)
{
	TERM *t;			/* pointer to the new term to be created */
	FORM *newf1;		/* pointer to the new form to be created */
	VARENTRY *boundvar; /* pointer to the entry for the bound variable */
	FORM *varform;		/* pointer to the bound variable form */

	boundvar = lookfor(id, body->vars);
	if (boundvar != NULL)
	{
		newf1 = allocate_form(LAMBDA, level);
		varform = boundvar->var;
		if ((varform->name == TRIANGLE) && (varform->nlevel[1] == 0))
		{
			connect(varform->nform[1],
					varform->nport[1],
					newf1, 2);
			myfree(varform);
		}
		else
			connect(varform, 0, newf1, 2);
		connect(newf1, 1, body->root_form, body->root_ports);
		t = allocate_term(newf1, 0, remv(id, body->vars));
	}
	else
	{
		newf1 = allocate_form(LAMBDAUNB, level);
		t = allocate_term(newf1, 0, body->vars);
		connect1(newf1, 1, body->root_form, body->root_ports);
	}
	free(body);
	return t;
}

TERM *buildplambdaterm(
	int level,
	PATTERN *pattern,
	TERM *body)
{
	TERM *t; /* pointer to the new term to be created */
	VARLIST *vp;
	FORM *newf1;
	FORM *newf2;		/* pointers to the new forms to be created */
	VARENTRY *boundvar; /* pointer to the entry for the bound variable */
	FORM *varform;		/* pointer to the bound variable form */
	bool boundp;

	for (vp = pattern->var_list, boundp = false; vp; vp = vp->next)
		if (lookfor(vp->id->id, body->vars))
		{
			boundp = true;
			break;
		}

	if (boundp)
	{
		newf1 = allocate_form(LAMBDA, level);
		connect(pattern->term->root_form, 0, newf1, 2);
		for (vp = pattern->var_list; vp != NULL; vp = vp->next)
		{
			if ((boundvar = lookfor(vp->id->id, body->vars)) == 0)
			{
				newf2 = allocate_form(ERASE, level);
				connect(newf2, 0, vp->id->form->nform[0], vp->id->form->nport[0]);
			}
			else
			{
				if ((varform = boundvar->var) == NULL)
				{
					fprintf(stderr, "You lose\n");
					exit(1974);
				}
				if ((varform->name == TRIANGLE) && (varform->nlevel[1] == 0))
				{
					connect(varform->nform[1],
							varform->nport[1],
							vp->id->form->nform[0],
							vp->id->form->nport[0]);
					myfree(varform);
				}
				else
				{
					connect(varform, 0,
							vp->id->form->nform[0],
							vp->id->form->nport[0]);
				}
			}
			myfree(vp->id->form);
		} /* for */
		t = allocate_term(newf1, 0, remvp(pattern->var_list, body->vars));
		connect(newf1, 1, body->root_form, body->root_ports);
	}
	else
	{
		/* apparent memory leak, but there's the destroyer */
		newf1 = allocate_form(LAMBDAUNB, level);
		t = allocate_term(newf1, 0, body->vars);
		connect1(newf1, 1, body->root_form, body->root_ports);
	}
	free(body);
	return t;
}

/* The following function creates the graph representation of a */
/* recursive definition. */
TERM *build_mu_term(
	int level,
	STBUCKET *id,
	TERM *body)

{
	TERM *t,
		/* pointer to the new term to be created */
		*temp;
	FORM *newf1; /* pointer to the new form to be created */
	VARENTRY *boundvar;
	/* pointer to the entry for the bound variable */
	FORM *varform;
	/* pointer to the bound variable form */

	boundvar = lookfor(id, body->vars);
	if (boundvar != NULL)
	{
		newf1 = allocate_form(FAN, level);
		newf1->nlevel[1] = -1;
		newf1->nlevel[2] = 1;
		varform = boundvar->var;

		if ((varform->name == TRIANGLE) &&
			(varform->nlevel[1] == 0))
		{
			connect(varform->nform[1],
					varform->nport[1],
					newf1, 2);
			myfree(varform);
		}
		else
			intelligent_connect(newf1, 2, varform);
		temp = allocate_term(newf1, 1, remv(id, body->vars));
		t = makebox(level, temp);
		connect(newf1, 0, body->root_form, body->root_ports);
	}
	else
	{
		newf1 = allocate_form(TRIANGLE, level);
		newf1->nlevel[1] = -1;
		connect1(newf1, 0, body->root_form, body->root_ports);
		temp = allocate_term(newf1, 1, body->vars);
		t = makebox(level, temp);
	}
	free(body);
	return t;
}

/* The following function creates the graph representation of */
/* an application */
TERM *buildappterm(
	int level,
	TERM *fun,
	TERM *arg)
{
	TERM *temp = makebox(level, arg);
	/* free variables of the application */
	FORM *newf = allocate_form(APP, level);

	connect1(newf, 0, fun->root_form, fun->root_ports);
	connect1(newf, 2, temp->root_form, temp->root_ports);
	VARENTRY *newvars = share(level, fun->vars, temp->vars);

	TERM *t = allocate_term(newf, 1, newvars);
	free(arg);
	free(fun);
	return t;
}

/* The following function creates the graph representation of */
/* an if_then_else term */
TERM *buildifelseterm(
	int level,
	TERM *arg1,
	TERM *arg2,
	TERM *arg3)
{
	TERM *t; /* pointer to the term to be created */
	VARENTRY *newvars, *tempvars;
	/* free variables of the application */
	FORM *newf,
		*newf1;
	/* pointers to the new forms */

	newf = allocate_form(IFELSE, level);
	newf1 = allocate_form(CONS, level);

	connect1(newf, 0, arg1->root_form, arg1->root_ports);
	connect(newf, 2, newf1, 0);
	connect1(newf1, 1, arg2->root_form, arg2->root_ports);
	connect1(newf1, 2, arg3->root_form, arg3->root_ports);

	tempvars = share(level, arg2->vars, arg3->vars);
	newvars = share(level, tempvars, arg1->vars);

	t = allocate_term(newf, 1, newvars);
	free(arg1);
	free(arg2);
	free(arg3);
	return t;
}

/* The following function creates the graph representation of */
/* a let_in expression*/
TERM *buildletinterm(
	int level,
	STBUCKET *id,
	TERM *arg1,
	TERM *arg2)
{
	TERM *temp;

	temp = buildlambdaterm(level, id, arg2);
	return buildappterm(level, temp, arg1);
}

/* The following function creates the graph representation of */
/* a boolean and-expression */
TERM *buildandterm(
	int level,
	TERM *arg1,
	TERM *arg2)
{
	TERM *t;
	/* pointer to the term to be created */
	VARENTRY *newvars;
	/* free variables of the application */
	FORM *newf;
	/* pointer to the new form to be created */

	newf = allocate_form(AND, level);

	connect1(newf, 0, arg1->root_form, arg1->root_ports);
	connect1(newf, 2, arg2->root_form, arg2->root_ports);

	newvars = share(level, arg1->vars, arg2->vars);

	t = allocate_term(newf, 1, newvars);
	free(arg1);
	free(arg2);
	return t;
}

/* The following function creates the graph representation of */
/* a boolean or-expression */
TERM *buildorterm(
	int level,
	TERM *arg1,
	TERM *arg2)
{
	TERM *t;
	/* pointer to the term to be created */
	VARENTRY *newvars;
	/* free variables of the application */
	FORM *newf;
	/* pointer to the new form to be created */

	newf = allocate_form(OR, level);

	connect1(newf, 0, arg1->root_form, arg1->root_ports);
	connect1(newf, 2, arg2->root_form, arg2->root_ports);

	newvars = share(level, arg1->vars, arg2->vars);

	t = allocate_term(newf, 1, newvars);
	free(arg1);
	free(arg2);
	return t;
}

/* The following function creates the graph representation of */
/* a boolean not-expression */
TERM *buildnotterm(
	int level,
	TERM *arg)
{
	TERM *t;
	/* pointer to the term to be created */

	FORM *newf;
	/* pointer to the new form to be created */

	newf = allocate_form(NOT, level);

	connect1(newf, 0, arg->root_form, arg->root_ports);

	t = allocate_term(newf, 1, arg->vars);
	free(arg);
	return t;
}

/* The following function creates the graph representation of */
/* a mat-expression */
TERM *buildmatterm(
	int level,
	TERM *arg1,
	TERM *arg2,
	int op)
{
	TERM *t;
	/* pointer to the term to be created */
	VARENTRY *newvars;
	/* free variables of the application */
	FORM *newf;
	/* pointer to the new form to be created */

	newf = allocate_form(op, level);
	if (arg1->root_ports == INT)
	{
		newf->nform[2] = arg1->root_form;
		switch (newf->name)
		{
		case ADD:
			newf->name = ADD1;
			break;
		case SUB:
			newf->name = SUB1;
			break;
		case PROD:
			newf->name = PROD1;
			break;
		case DIV:
			newf->name = DIV1;
			break;
		case MOD:
			newf->name = MOD1;
			break;
		}
		connect1(newf, 0, arg2->root_form, arg2->root_ports);
		t = allocate_term(newf, 1, arg2->vars);
	}
	else
	{
		if (arg2->root_ports == INT && op != DIV && op != MOD)
		{
			newf->nform[2] = arg2->root_form;
			switch (newf->name)
			{
			case ADD:
				newf->name = ADD1;
				break;
			case SUB:
				newf->name = ADD1;
				newf->nform[2] = (FORM *)-(long int)newf->nform[2];
				break;
			case PROD:
				newf->name = PROD1;
				break;
			}
			connect1(newf, 0, arg1->root_form, arg1->root_ports);
			t = allocate_term(newf, 1, arg1->vars);
		}
		else
		{
			connect1(newf, 0, arg1->root_form, arg1->root_ports);
			connect1(newf, 2, arg2->root_form, arg2->root_ports);
			newvars = share(level, arg1->vars, arg2->vars);
			t = allocate_term(newf, 1, newvars);
		}
	}
	free(arg1);
	free(arg2);
	return t;
}

/* The following function creates the graph representation of */
/* a unary minus-expression */
TERM *buildminusterm(
	int level,
	TERM *arg1)
{
	TERM *t;
	/* pointer to the term to be created */
	FORM *newf;
	/* pointer to the new form to be created */

	if (arg1->root_ports == INT)
	{
		arg1->root_form = (FORM *)(-(long int)arg1->root_form);
		t = arg1;
	}
	else
	{
		newf = allocate_form(SUB1, level);
		newf->num_safe = 0;
		connect1(newf, 0, arg1->root_form, arg1->root_ports);
		t = allocate_term(newf, 1, arg1->vars);
		free(arg1);
	}
	return t;
}

/* The following function creates the graph representation of */
/* a relop-expression */
TERM *buildrelopterm(
	int level,
	TERM *arg1,
	TERM *arg2,
	int relop)
{
	TERM *t;
	/* pointer to the term to be created */
	VARENTRY *newvars;
	/* free variables of the application */
	FORM *newf;
	/* pointer to the new form to be created */

	newf = allocate_form(relop, level);
	if (arg1->root_ports == INT)
	{
		newf->nform[2] = arg1->root_form;
		switch (newf->name)
		{
		case LESS:
			newf->name = LESS1;
			break;
		case EQ:
			newf->name = EQ1;
			break;
		case MORE:
			newf->name = MORE1;
			break;
		case LEQ:
			newf->name = LEQ1;
			break;
		case MEQ:
			newf->name = MEQ1;
			break;
		case NOTEQ:
			newf->name = NOTEQ1;
			break;
		}
		connect1(newf, 0, arg2->root_form, arg2->root_ports);
		t = allocate_term(newf, 1, arg2->vars);
	}
	else
	{
		if (arg2->root_ports == INT)
		{
			newf->nform[2] = arg2->root_form;
			switch (newf->name)
			{
			case LESS:
				newf->name = MORE1;
				break;
			case EQ:
				newf->name = EQ1;
				break;
			case MORE:
				newf->name = LESS1;
				break;
			case LEQ:
				newf->name = MEQ1;
				break;
			case MEQ:
				newf->name = LEQ1;
				break;
			case NOTEQ:
				newf->name = NOTEQ1;
				break;
			}
			connect1(newf, 0, arg1->root_form, arg1->root_ports);
			t = allocate_term(newf, 1, arg1->vars);
		}
		else
		{
			connect1(newf, 0, arg1->root_form, arg1->root_ports);
			connect1(newf, 2, arg2->root_form, arg2->root_ports);
			newvars = share(level, arg1->vars, arg2->vars);
			t = allocate_term(newf, 1, newvars);
		}
	}
	free(arg1);
	free(arg2);
	return t;
}

TERM *buildnillist(int level)
{
	return allocate_term(NULL, NIL, NULL);
}

TERM *buildlist(
	int level,
	TERM *arg1,
	TERM *arg2)
{
	TERM *t;	 /* pointer to the term to be created */
	FORM *newf1; /* pointer to the new form to be created */
	VARENTRY *newvars;

	if (arg2 != NULL)
	{

		newf1 = allocate_form(CONS, level);

		connect1(newf1, 1, arg1->root_form, arg1->root_ports);
		connect1(newf1, 2, arg2->root_form, arg2->root_ports);

		newvars = share(level, arg1->vars, arg2->vars);

		t = allocate_term(newf1, 0, newvars);
	}
	else
	{
		newf1 = allocate_form(CONS, level);

		connect1(newf1, 1, arg1->root_form, arg1->root_ports);
		bool_connect(newf1, 2, NIL);

		t = allocate_term(newf1, 0, arg1->vars);
	}

	free(arg1);
	free(arg2);
	return t;
}

TERM *buildlist1(
	int level,
	TERM *arg1,
	TERM *arg2)
{
	TERM *t;	 /* pointer to the term to be created */
	FORM *newf1; /* pointer to the new form to be created */
	VARENTRY *newvars;

	if (arg2 != NULL)
	{

		newf1 = allocate_form(CONS1, level);

		connect1(newf1, 1, arg1->root_form, arg1->root_ports);
		connect1(newf1, 2, arg2->root_form, arg2->root_ports);

		newvars = share(level, arg1->vars, arg2->vars);

		t = allocate_term(newf1, 0, newvars);
	}
	else
	{
		newf1 = allocate_form(CONS1, level);

		connect1(newf1, 1, arg1->root_form, arg1->root_ports);
		bool_connect(newf1, 2, NIL);

		t = allocate_term(newf1, 0, arg1->vars);
	}

	free(arg1);
	free(arg2);
	return t;
}

TERM *buildcarterm(
	int level,
	TERM *arg)
{
	FORM *newf;
	/* pointer to the new form to be created */

	newf = allocate_form(CAR, level);

	connect1(newf, 0, arg->root_form, arg->root_ports);

	TERM *t = allocate_term(newf, 1, arg->vars);
	free(arg);
	return t;
}

TERM *buildcdrterm(
	int level,
	TERM *arg)
{
	FORM *newf;
	/* pointer to the new form to be created */

	newf = allocate_form(CDR, level);

	connect1(newf, 0, arg->root_form, arg->root_ports);

	TERM *t = allocate_term(newf, 1, arg->vars);
	free(arg);
	return t;
}

TERM *buildtestnil(
	int level,
	TERM *arg)
{

	FORM *newf;
	/* pointer to the new form to be created */

	newf = allocate_form(TESTNIL, level);

	connect1(newf, 0, arg->root_form, arg->root_ports);

	TERM *t = allocate_term(newf, 1, arg->vars);
	free(arg);
	return t;
}

/* the following function adds a root node to a term */
FORM *closeterm(
	int level,
	TERM *t)
{
	if (t == NULL)
	{
		return NULL;
	}
	FORM *newroot = allocate_form(ROOT, 0);

	connect1(newroot, 0, t->root_form, t->root_ports);

	if (level == 1)
		t->vars = addbrackets(0, t->vars);
	closeglobalvars(t->vars);
	return newroot;
}

/* the following function allocate a new graphical form */
/* and initialize the name and index fields */
FORM *allocate_form(
	/* name of the form */
	int name,
	/* index of the form */
	int index)

{
	if (headfree->next == NULL)
	{
		FORM *dep;
		headfree->next = dep = (FORM *)malloc_da(sizeof(FORM) * FORM_NUM);
		dep->next = dep + 1;
		dep->prev = headfree;
		dep = dep->next;
		for (int i = 2; i < FORM_NUM; i++)
		{
			dep->next = dep + 1;
			dep->prev = dep - 1;
			dep = dep->next;
		}
		dep->next = NULL;
		dep->prev = dep - 1;
	}
	FORM *form = headfree;
	headfree = headfree->next;
	num_nodes++;
	if (num_nodes > max_nodes)
		max_nodes = num_nodes;
	form->name = name;
	form->index = index;
	form->nform[0] = NULL;
	form->nform[1] = NULL;
	form->nform[2] = NULL;
	form->num_safe = true; /* initially, all operators are safe */
	return form;
}

/* the following function adds a graphical form to deallocate */
/* in a list of free forms (i.e a free-list of forms)         */
void myfree(
	/* pointer to the form to deallocate      */
	FORM *form)
{
	form->prev->next = form->next;
	form->next->prev = form->prev;
	num_nodes--;
	form->next = headfree->next;
	form->prev = headfree;
	if (headfree->next != NULL)
		headfree->next->prev = form;
	headfree->next = form;
}

/* the following function connects together the port portf1 of */
/* form1 to the port portf2 of form2 */
void connect(
	FORM *form1,
	int portf1,
	FORM *form2,
	int portf2)
{
	form1->nport[portf1] = portf2;
	form1->nform[portf1] = form2;
	form2->nport[portf2] = portf1;
	form2->nform[portf2] = form1;
}

/* the following function connects the port portf1 of form1 to	*/
/* the port portf2 of form2 and vice versa if the form2 is not */
/* a NIL, INT, True or False form.				*/
void connect1(
	FORM *form1,
	int portf1,
	FORM *form2,
	int portf2)
{
	form1->nport[portf1] = portf2;
	form1->nform[portf1] = form2;
	if (portf2 >= 0)
	{
		form2->nport[portf2] = portf1;
		form2->nform[portf2] = form1;
	}
}

/* the following function connects only the port portf1 of 	*/
/* form1 to the port portf2 of form2, because form2 is a INT	*/
void int_connect(
	FORM *form1,
	int portf1,
	FORM *form2,
	int portf2)
{
	form1->nport[portf1] = portf2;
	form1->nform[portf1] = form2;
}

/* the following function connects only the port portf1 of 	*/
/* form1 to the port portf2 of form2, because form2 is a INT,	*/
/* NIL, True or False.						*/
void bool_connect(
	FORM *form1,
	int portf1,
	int portf2)
{
	form1->nport[portf1] = portf2;
}

/****************************************************************/
/* 6. Definitions of functions strictly local to the module.	*/
/****************************************************************/

/* the following function allocate a new variable entry */
static VARENTRY *allocate_var(
	/* identifier of the variable */
	STBUCKET *id,
	/* graphical form for the variable */
	FORM *form,
	/* pointer to the next free variable */
	VARENTRY *nextvar)
{
	/* reference to the pointer of the */
	/* free variable entry to be created */
	VARENTRY *newvar = (VARENTRY *)malloc_da(sizeof(VARENTRY));
	newvar->name = id;
	newvar->var = form;
	newvar->next = nextvar;
	return newvar;
}

/* the following function allocate a new term entry */
TERM *allocate_term(
	/* pointer to the root form of the term */
	FORM *root_form,
	/* root port of the term */
	int root_ports,
	/* pointer to the free variables entries */
	/* of the term */
	VARENTRY *freevars)
{
	TERM *term = (TERM *)malloc_da(sizeof(TERM));
	term->root_form = root_form;
	term->root_ports = root_ports;
	term->vars = freevars;
	return term;
}

/* the following function build a box around a term  */
static TERM *makebox(int level, TERM *arg)
{
	arg->vars = addbrackets(level, arg->vars);
	return arg;
}

/* the following function add a sequence of square brackets of */
/* given index at the free variables in listvar */
static VARENTRY *addbrackets(
	int index,
	VARENTRY *listvar)
{
	if (listvar == NULL)
	{
		return NULL;
	}
	/* resulting variable entry list  */
	FORM *variab;
	FORM *bracket;
	/* new form to be created */

	variab = listvar->var;
	if (variab->name != CONS1)
	{
		switch (variab->name)
		{
		case TRIANGLE:
			variab->index = index;
			variab->nlevel[1] = variab->nlevel[1] + 1;
			break;
		case CDR1:
		case CAR1:
		case TESTNIL1:
		case FAN:
			variab->index--;
			variab->nlevel[1] = variab->nlevel[1] + 1;
			variab->nlevel[2] = variab->nlevel[2] + 1;
			break;
		};
		listvar->next = addbrackets(index, listvar->next);
		return listvar;
	}
	else
	{
		bracket = allocate_form(TRIANGLE, index);
		bracket->nlevel[1] = 1;
		connect(bracket, 1, variab, 0);
		return allocate_var(listvar->name,
							bracket,
							addbrackets(index, listvar->next));
	}
}

/* The following function shares the free variables of	*/
/* two terms, by adding suitable FANS.  		*/
static VARENTRY *share(
	int index,
	/* pointers to the lists of variables to be shared */
	VARENTRY *l1,
	VARENTRY *l2)

{
	if (l1 == NULL)
	{
		return l2;
	}
	VARENTRY *res;

	VARENTRY *var = lookfor(l1->name, l2);
	if (var == NULL)
	{
		res = l1;
		res->next = share(index, l1->next, l2);
		return res;
	}

	FORM *fan = allocate_form(FAN, index);
	fan->nlevel[1] = 0;
	fan->nlevel[2] = 0;

	intelligent_connect(fan, 1, l1->var);
	intelligent_connect(fan, 2, var->var);

	res = l1;
	res->name = l1->name;
	res->var = fan;
	res->next = share(index, l1->next, remv(l1->name, l2));
	return res;
}

/* The following function searches for a variable inside a list. */
static VARENTRY *lookfor(
	STBUCKET *id,
	/* pointer to the identifier to be found  */
	VARENTRY *listvar)
/* pointer to the variable list to be scanned */
{
	if (listvar == NULL)
		return NULL;
	else if (listvar->name == id)
		return listvar;
	else
		return lookfor(id, listvar->next);
}

/* the following function remove an identifier form a list */
/* of variables */
static VARENTRY *remv(
	STBUCKET *id,
	/* pointer to the identifier to be removed  */
	VARENTRY *listvar)
/* pointer to the variable list to be scanned */
{
	VARENTRY *temp;
	if (listvar == NULL)
	{
		return NULL;
	}
	if (id == listvar->name)
	{
		temp = listvar->next;
		free(listvar);
		return temp;
	}
	else
	{
		listvar->next = remv(id, listvar->next);
		return listvar;
	}
}

/* the following functions does the set substraction of two variable lists */
/* it runs in quadratic time, but who cares? */
static VARENTRY *remvp(
	VARLIST *vl,
	VARENTRY *listvar) /* pointer to the variable list to be scanned */
{
	;
	for (VARLIST *v = vl; v; v = v->next)
	{
		listvar = remv(v->id->id, listvar);
	}
	return listvar;
}

/* The following function copies all the graph of the global */
/* definition.	*/
static void closeglobalvars(
	VARENTRY *listvar)
/* pointer to the variable list to be scanned */
{
	if (listvar != NULL)
	{
		const FORM *formvar = listvar->var;
		const FORM *formterm = ((listvar->name)->curr_binding)->root;
		if (formvar->name == TRIANGLE)
		{
			FORM *newf = copy(formterm->nform[0],
							  formterm->nport[0],
							  formvar->nlevel[1]);
			connect1(formvar->nform[1], formvar->nport[1],
					 newf, formterm->nport[0]);
		}
		else
		{
			FORM *newf = copy(formterm->nform[0], formterm->nport[0], 0);
			connect1(formvar, 0,
					 newf, formterm->nport[0]);
		}
		closeglobalvars(listvar->next);
	}
}

/* The following function tries to merge two forms into a single one. */
/* If failing doing so, connects them normally        		      */

static void intelligent_connect(
	FORM *f1,
	int port,
	FORM *f2)
{
	int dep;
	FORM *new;

	switch (f1->name)
	{
	case FAN:
		switch (f2->name)
		{
		case TRIANGLE:
			f1->nlevel[port] += f2->nlevel[1];
			inspect_connect(f1, port, f2->nform[1], f2->nport[1]);
			myfree(f2);
			break;
		case TESTNIL:
			f1->name = TESTNIL1;
			f1->index = f2->index - f1->nlevel[port];
			if (port == 2)
			{
				dep = f1->nlevel[1];
				f1->nlevel[1] = f1->nlevel[2];
				f1->nlevel[2] = dep;
				inspect_connect(f1, 2, f1->nform[1], f1->nport[1]);
			}
			inspect_connect(f1, 1, f2->nform[1], f2->nport[1]);
			myfree(f2);
			break;
		case CAR:
			f1->name = CAR1;
			f1->index = f2->index - f1->nlevel[port];
			if (port == 2)
			{
				dep = f1->nlevel[1];
				f1->nlevel[1] = f1->nlevel[2];
				f1->nlevel[2] = dep;
				inspect_connect(f1, 2, f1->nform[1], f1->nport[1]);
			}
			inspect_connect(f1, 1, f2->nform[1], f2->nport[1]);
			myfree(f2);
			break;
		case CDR:
			f1->name = CDR1;
			f1->index = f2->index - f1->nlevel[port];
			if (port == 2)
			{
				dep = f1->nlevel[1];
				f1->nlevel[1] = f1->nlevel[2];
				f1->nlevel[2] = dep;
				inspect_connect(f1, 2, f1->nform[1], f1->nport[1]);
			}
			inspect_connect(f1, 1, f2->nform[1], f2->nport[1]);
			myfree(f2);
			break;
		default:
			connect(f1, port, f2, 0);
			break;
		}
		break;
	case TESTNIL1:
		switch (f2->name)
		{
		case TRIANGLE:
			f1->nlevel[port] += f2->nlevel[1];
			inspect_connect(f1, port, f2->nform[1], f2->nport[1]);
			myfree(f2);
			break;
		default:
			connect(f1, port, f2, 0);
			break;
		}
		break;
	case CAR1:
		switch (f2->name)
		{
		case TRIANGLE:
			f1->nlevel[port] += f2->nlevel[1];
			inspect_connect(f1, port, f2->nform[1], f2->nport[1]);
			myfree(f2);
			break;
		case CDR:
			if (port == 2)
			{
				f1->name = CONS1;
				if (f1->nlevel[1] != 0)
				{
					new = allocate_form(TRIANGLE, f1->index - 1);
					new->nlevel[1] = f1->nlevel[1];
					connect1(new, 1, f1->nform[1], f1->nport[1]);
					connect(f1, 1, new, 0);
				}
				if (f1->nlevel[2] != 0)
				{
					f2->name = TRIANGLE;
					f2->nlevel[1] = f1->nlevel[2];
					f2->index = f1->index - 1;
					connect(f1, 2, f2, 0);
				}
				else
				{
					connect1(f1, 2, f2->nform[1], f2->nport[1]);
					myfree(f2);
				}
			}
			else
				connect(f1, port, f2, 0);
			break;
		case TESTNIL:
			if (port == 2)
			{
				f1->name = TESTNIL1;
				f2->name = CAR;
				dep = f2->index;
				f2->index = f1->index + f1->nlevel[1];
				f1->index = dep - f1->nlevel[2];
				dep = f1->nlevel[1];
				f1->nlevel[1] = f1->nlevel[2];
				f1->nlevel[2] = dep;
				new = f1->nform[1];
				dep = f1->nport[1];
				connect1(f1, 1, f2->nform[1], f2->nport[1]);
				connect1(f2, 1, new, dep);
				connect(f2, 0, f1, 2);
			}
			else
				connect(f1, port, f2, 0);
			break;
		default:
			connect(f1, port, f2, 0);
			break;
		}
		break;
	case CDR1:
		switch (f2->name)
		{
		case TRIANGLE:
			f1->nlevel[port] += f2->nlevel[1];
			inspect_connect(f1, port, f2->nform[1], f2->nport[1]);
			myfree(f2);
			break;
		case CAR:
			if (port == 2)
			{
				f1->name = CONS1;
				if (f1->nlevel[1] != 0)
				{
					new = allocate_form(TRIANGLE, f1->index - 1);
					new->nlevel[1] = f1->nlevel[1];
					connect1(new, 1, f1->nform[1], f1->nport[1]);
					connect(f1, 2, new, 0);
				}
				else
					connect(f1, 2, f1->nform[1], f1->nport[1]);
				if (f1->nlevel[2] != 0)
				{
					f2->name = TRIANGLE;
					f2->nlevel[1] = f1->nlevel[2];
					f2->index = f1->index - 1;
					connect(f1, 1, f2, 0);
				}
				else
				{
					connect1(f1, 1, f2->nform[1], f2->nport[1]);
					myfree(f2);
				}
			}
			else
				connect(f1, port, f2, 0);
			break;
		case TESTNIL:
			if (port == 2)
			{
				f1->name = TESTNIL1;
				f2->name = CDR;
				dep = f2->index;
				f2->index = f1->index + f1->nlevel[1];
				f1->index = dep - f1->nlevel[2];
				dep = f1->nlevel[1];
				f1->nlevel[1] = f1->nlevel[2];
				f1->nlevel[2] = dep;
				new = f1->nform[1];
				dep = f1->nport[1];
				connect1(f1, 1, f2->nform[1], f2->nport[1]);
				connect1(f2, 1, new, dep);
				connect(f2, 0, f1, 2);
			}
			else
				connect(f1, port, f2, 0);
			break;
		default:
			connect(f1, port, f2, 0);
			break;
		}
		break;
	default:
		connect(f1, port, f2, 0);
		break;
	}
}

/* The following function checks whether it's possible to apply      */
/* function intelligent_connect. Otherwise applies a normal connect. */
static void inspect_connect(
	FORM *f1,
	int p1,
	FORM *f2,
	int p2)
{
	if (p2 == 0)
		intelligent_connect(f1, p1, f2);
	else
		connect1(f1, p1, f2, p2);
}

static bool membervarlist(
	BINDINGID *e,
	VARLIST *l)
{
	while (l)
	{
		if (e->id == l->id->id)
		{
			return true;
		}
		l = l->next;
	}
	return false;
}

VARLIST *mergevarlist(
	VARLIST *l1, VARLIST *l2)
{
	VARLIST *p;
	if (l1)
	{
		for (p = l1; p->next; p = p->next)
		{
			if (membervarlist(p->id, l2))
			{
				fprintf(stderr, "Pattern is not linear - expect garbage out!\n");
			}
		}
		if (membervarlist(p->id, l2))
		{
			fprintf(stderr, "Pattern is not linear - expect garbage out!\n");
		}
		p->next = l2;
		return l1;
	}
	else
		return l2;
}

VARLIST *makevarlist(
	STBUCKET *e,
	TERM *t)
{
	BINDINGID *bid;
	VARLIST *vl;
	bid = (BINDINGID *)malloc(sizeof(BINDINGID));
	bid->id = e;
	bid->form = t->root_form;
	vl = (VARLIST *)malloc(sizeof(VARLIST));
	vl->next = NULL;
	vl->id = bid;
	return vl;
}

TERM *buildvoidterm(int level)

{
	FORM *newf;
	newf = allocate_form(TRIANGLE, level);
	newf->nlevel[1] = 0;
	return allocate_term(newf, 0, NULL);
}

void free_pattern(
	PATTERN *p)
{
	VARLIST *vl, *vln;
	for (vl = p->var_list; vl; vl = vln)
	{
		vln = vl->next;
		free(vl);
	}
	free(p);
}
