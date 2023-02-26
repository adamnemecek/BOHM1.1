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
/*  -new FORM(): it allocates a new graphical form.       */
/* The functions buildvarterm(), buildplambdaterm(),            */
/* buildappterm() and buildletinterm() etc. are called by the   */
/* parser.                                                      */
/* The functions connect() andnew FORM() are also used    */
/* during reduction.                                            */
/* The following functions are local to the module:             */
/*  - allocate_var(): it allocates a new entry for a variable.  */
/*  - new TERM(): it allocates a new entry for a term.     */
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

/* The following function creates the graph representation of */
/* a variable */
TERM *buildvarterm(
	int level,
	STBUCKET *id)
{
	/* pointer to the new form to be created */
	FORM *newf = new FORM(TRIANGLE, level);
	newf->nlevel[1] = -1;

	/* pointer to the new free variable entry */
	VARENTRY *newvar = new VARENTRY(id, newf, NULL);
	return new TERM(newf, 1, newvar);
}

/* The following function creates the graph representation of */
/* a true constant */
TERM *buildtrueterm(int level)
{
	return new TERM(NULL, T, NULL);
}

/* The following function creates the graph representation of */
/* a false constant */
TERM *buildfalseterm(int level)
{
	return new TERM(NULL, F, NULL);
}

/* The following function creates the graph representation of */
/* a numerical constant */
TERM *buildintterm(
	int level,
	long int value)
{
	return new TERM((FORM *)value, INT, NULL);
}

/* The following function creates the graph representation of a */
/* lambda-abstraction */
static TERM *buildlambdaterm(
	int level,
	STBUCKET *id,
	TERM *body)
{
	TERM *t; /* pointer to the new TERM to be created */

	VARENTRY *boundvar = lookfor(id, body->vars);
	if (boundvar != NULL)
	{
		FORM *newf1 = new FORM(LAMBDA, level);
		FORM *varform = boundvar->var;
		if (varform->kind == TRIANGLE && varform->nlevel[1] == 0)
		{
			connect(varform->nform[1],
					varform->nport[1],
					newf1, 2);
			varform->release();
		}
		else
			connect(varform, 0, newf1, 2);
		connect(newf1, 1, body->root_form, body->root_ports);
		t = new TERM(newf1, 0, remv(id, body->vars));
	}
	else
	{
		FORM *newf1 = new FORM(LAMBDAUNB, level);
		t = new TERM(newf1, 0, body->vars);
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
	TERM *t; /* pointer to the new TERM to be created */
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
		newf1 = new FORM(LAMBDA, level);
		connect(pattern->term->root_form, 0, newf1, 2);
		for (vp = pattern->var_list; vp != NULL; vp = vp->next)
		{
			if ((boundvar = lookfor(vp->id->id, body->vars)) == 0)
			{
				newf2 = new FORM(ERASE, level);
				connect(newf2, 0, vp->id->form->nform[0], vp->id->form->nport[0]);
			}
			else
			{
				if ((varform = boundvar->var) == NULL)
				{
					fprintf(stderr, "You lose\n");
					exit(1974);
				}
				if (varform->kind == TRIANGLE && varform->nlevel[1] == 0)
				{
					connect(varform->nform[1],
							varform->nport[1],
							vp->id->form->nform[0],
							vp->id->form->nport[0]);
					varform->release();
				}
				else
				{
					connect(varform, 0,
							vp->id->form->nform[0],
							vp->id->form->nport[0]);
				}
			}
			vp->id->form->release();
		} /* for */
		t = new TERM(newf1, 0, remvp(pattern->var_list, body->vars));
		connect(newf1, 1, body->root_form, body->root_ports);
	}
	else
	{
		/* apparent memory leak, but there's the destroyer */
		newf1 = new FORM(LAMBDAUNB, level);
		t = new TERM(newf1, 0, body->vars);
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
		/* pointer to the new TERM to be created */
		*temp;
	FORM *newf1; /* pointer to the new form to be created */

	FORM *varform;
	/* pointer to the bound variable form */

	/* pointer to the entry for the bound variable */
	VARENTRY *boundvar = lookfor(id, body->vars);
	if (boundvar != NULL)
	{
		newf1 = new FORM(FAN, level);
		newf1->nlevel[1] = -1;
		newf1->nlevel[2] = 1;
		varform = boundvar->var;

		if ((varform->kind == TRIANGLE) &&
			(varform->nlevel[1] == 0))
		{
			connect(varform->nform[1],
					varform->nport[1],
					newf1, 2);
			varform->release();
		}
		else
			intelligent_connect(newf1, 2, varform);
		temp = new TERM(newf1, 1, remv(id, body->vars));
		t = makebox(level, temp);
		connect(newf1, 0, body->root_form, body->root_ports);
	}
	else
	{
		newf1 = new FORM(TRIANGLE, level);
		newf1->nlevel[1] = -1;
		connect1(newf1, 0, body->root_form, body->root_ports);
		temp = new TERM(newf1, 1, body->vars);
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
	FORM *newf = new FORM(APP, level);

	connect1(newf, 0, fun->root_form, fun->root_ports);
	connect1(newf, 2, temp->root_form, temp->root_ports);
	VARENTRY *newvars = share(level, fun->vars, temp->vars);

	TERM *t = new TERM(newf, 1, newvars);
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
	/* free variables of the application */
	FORM *newf = new FORM(IFELSE, level);
	FORM *newf1 = new FORM(CONS, level);
	/* pointers to the new forms */

	connect1(newf, 0, arg1->root_form, arg1->root_ports);
	connect(newf, 2, newf1, 0);
	connect1(newf1, 1, arg2->root_form, arg2->root_ports);
	connect1(newf1, 2, arg3->root_form, arg3->root_ports);

	VARENTRY *tempvars = share(level, arg2->vars, arg3->vars);
	VARENTRY *newvars = share(level, tempvars, arg1->vars);

	/* pointer to the term to be created */
	TERM *t = new TERM(newf, 1, newvars);
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
	TERM *temp = buildlambdaterm(level, id, arg2);
	return buildappterm(level, temp, arg1);
}

/* The following function creates the graph representation of */
/* a boolean and-expression */
TERM *buildandterm(
	int level,
	TERM *arg1,
	TERM *arg2)
{
	/* pointer to the new form to be created */
	FORM *newf = new FORM(AND, level);

	connect1(newf, 0, arg1->root_form, arg1->root_ports);
	connect1(newf, 2, arg2->root_form, arg2->root_ports);

	/* free variables of the application */
	VARENTRY *newvars = share(level, arg1->vars, arg2->vars);

	/* pointer to the term to be created */
	TERM *t = new TERM(newf, 1, newvars);
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
	/* pointer to the new form to be created */
	FORM *newf = new FORM(OR, level);

	connect1(newf, 0, arg1->root_form, arg1->root_ports);
	connect1(newf, 2, arg2->root_form, arg2->root_ports);

	/* free variables of the application */
	VARENTRY *newvars = share(level, arg1->vars, arg2->vars);

	/* pointer to the term to be created */
	TERM *t = new TERM(newf, 1, newvars);
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
	/* pointer to the new form to be created */
	FORM *newf = new FORM(NOT, level);

	connect1(newf, 0, arg->root_form, arg->root_ports);

	/* pointer to the term to be created */
	TERM *t = new TERM(newf, 1, arg->vars);
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
	/* pointer to the term to be created */
	TERM *t;

	/* pointer to the new form to be created */
	FORM *newf = new FORM(op, level);
	if (arg1->root_ports == INT)
	{
		newf->nform[2] = arg1->root_form;
		switch (newf->kind)
		{
		case ADD:
			newf->kind = ADD1;
			break;
		case SUB:
			newf->kind = SUB1;
			break;
		case PROD:
			newf->kind = PROD1;
			break;
		case DIV:
			newf->kind = DIV1;
			break;
		case MOD:
			newf->kind = MOD1;
			break;
		}
		connect1(newf, 0, arg2->root_form, arg2->root_ports);
		t = new TERM(newf, 1, arg2->vars);
	}
	else
	{
		if (arg2->root_ports == INT && op != DIV && op != MOD)
		{
			newf->nform[2] = arg2->root_form;
			switch (newf->kind)
			{
			case ADD:
				newf->kind = ADD1;
				break;
			case SUB:
				newf->kind = ADD1;
				newf->nform[2] = (FORM *)-(long int)newf->nform[2];
				break;
			case PROD:
				newf->kind = PROD1;
				break;
			}
			connect1(newf, 0, arg1->root_form, arg1->root_ports);
			t = new TERM(newf, 1, arg1->vars);
		}
		else
		{
			connect1(newf, 0, arg1->root_form, arg1->root_ports);
			connect1(newf, 2, arg2->root_form, arg2->root_ports);
			/* free variables of the application */

			VARENTRY *newvars = share(level, arg1->vars, arg2->vars);
			t = new TERM(newf, 1, newvars);
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
	/* pointer to the term to be created */
	if (arg1->root_ports == INT)
	{
		arg1->root_form = (FORM *)(-(long int)arg1->root_form);
		return arg1;
	}
	/* pointer to the new form to be created */
	FORM *newf = new FORM(SUB1, level);
	newf->num_safe = 0;
	connect1(newf, 0, arg1->root_form, arg1->root_ports);
	TERM *t = new TERM(newf, 1, arg1->vars);
	free(arg1);
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
	/* pointer to the term to be created */
	TERM *t;

	/* pointer to the new form to be created */
	FORM *newf = new FORM(relop, level);
	if (arg1->root_ports == INT)
	{
		newf->nform[2] = arg1->root_form;
		switch (newf->kind)
		{
		case LESS:
			newf->kind = LESS1;
			break;
		case EQ:
			newf->kind = EQ1;
			break;
		case MORE:
			newf->kind = MORE1;
			break;
		case LEQ:
			newf->kind = LEQ1;
			break;
		case MEQ:
			newf->kind = MEQ1;
			break;
		case NOTEQ:
			newf->kind = NOTEQ1;
			break;
		}
		connect1(newf, 0, arg2->root_form, arg2->root_ports);
		t = new TERM(newf, 1, arg2->vars);
	}
	else
	{
		if (arg2->root_ports == INT)
		{
			newf->nform[2] = arg2->root_form;
			switch (newf->kind)
			{
			case LESS:
				newf->kind = MORE1;
				break;
			case EQ:
				newf->kind = EQ1;
				break;
			case MORE:
				newf->kind = LESS1;
				break;
			case LEQ:
				newf->kind = MEQ1;
				break;
			case MEQ:
				newf->kind = LEQ1;
				break;
			case NOTEQ:
				newf->kind = NOTEQ1;
				break;
			}
			connect1(newf, 0, arg1->root_form, arg1->root_ports);
			t = new TERM(newf, 1, arg1->vars);
		}
		else
		{
			connect1(newf, 0, arg1->root_form, arg1->root_ports);
			connect1(newf, 2, arg2->root_form, arg2->root_ports);
			/* free variables of the application */
			VARENTRY *newvars = share(level, arg1->vars, arg2->vars);
			t = new TERM(newf, 1, newvars);
		}
	}
	free(arg1);
	free(arg2);
	return t;
}

TERM *buildnillist(int level)
{
	return new TERM(NULL, NIL, NULL);
}

TERM *buildlist(
	int level,
	TERM *arg1,
	TERM *arg2)
{
	TERM *t;	 /* pointer to the term to be created */
	FORM *newf1; /* pointer to the new form to be created */

	if (arg2 != NULL)
	{
		newf1 = new FORM(CONS, level);

		connect1(newf1, 1, arg1->root_form, arg1->root_ports);
		connect1(newf1, 2, arg2->root_form, arg2->root_ports);

		VARENTRY *newvars = share(level, arg1->vars, arg2->vars);

		t = new TERM(newf1, 0, newvars);
	}
	else
	{
		newf1 = new FORM(CONS, level);

		connect1(newf1, 1, arg1->root_form, arg1->root_ports);
		bool_connect(newf1, 2, NIL);

		t = new TERM(newf1, 0, arg1->vars);
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

	if (arg2 != NULL)
	{

		newf1 = new FORM(CONS1, level);

		connect1(newf1, 1, arg1->root_form, arg1->root_ports);
		connect1(newf1, 2, arg2->root_form, arg2->root_ports);

		VARENTRY *newvars = share(level, arg1->vars, arg2->vars);

		t = new TERM(newf1, 0, newvars);
	}
	else
	{
		newf1 = new FORM(CONS1, level);

		connect1(newf1, 1, arg1->root_form, arg1->root_ports);
		bool_connect(newf1, 2, NIL);

		t = new TERM(newf1, 0, arg1->vars);
	}

	free(arg1);
	free(arg2);
	return t;
}

TERM *buildcarterm(
	int level,
	TERM *arg)
{
	/* pointer to the new form to be created */
	FORM *newf = new FORM(CAR, level);
	connect1(newf, 0, arg->root_form, arg->root_ports);

	TERM *t = new TERM(newf, 1, arg->vars);
	free(arg);
	return t;
}

TERM *buildcdrterm(
	int level,
	TERM *arg)
{
	/* pointer to the new form to be created */
	FORM *newf = new FORM(CDR, level);
	connect1(newf, 0, arg->root_form, arg->root_ports);

	TERM *t = new TERM(newf, 1, arg->vars);
	free(arg);
	return t;
}

TERM *buildtestnil(
	int level,
	TERM *arg)
{
	/* pointer to the new form to be created */
	FORM *newf = new FORM(TESTNIL, level);
	connect1(newf, 0, arg->root_form, arg->root_ports);

	TERM *t = new TERM(newf, 1, arg->vars);
	free(arg);
	return t;
}

/* the following function adds a root node to a term */
FORM *TERM::close(
	int level)
{
	FORM *newroot = new FORM(ROOT, 0);

	connect1(
		newroot,
		0,
		this->root_form,
		this->root_ports);

	if (level == 1)
		this->vars = addbrackets(0, this->vars);
	closeglobalvars(this->vars);
	return newroot;
}

/* the following function allocate a new graphical form */
/* and initialize the name and index fields */
FORM::FORM(
	/* name of the form */
	int kind,
	/* index of the form */
	int index)
{
	if (headfree->next == NULL)
	{
		FORM *dep = (FORM *)malloc_da(sizeof(FORM) * FORM_NUM);
		// auto *dep = new FORM[FORM_NUM];
		headfree->next = dep;
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
	*this = *headfree;
	headfree = headfree->next;
	num_nodes++;
	if (num_nodes > max_nodes)
		max_nodes = num_nodes;
	this->kind = kind;
	this->index = index;
	this->nform[0] = NULL;
	this->nform[1] = NULL;
	this->nform[2] = NULL;
	this->num_safe = true; /* initially, all operators are safe */
						   // return form;
}

/* the following function adds a graphical form to deallocate */
/* in a list of free forms (i.e a free-list of forms)         */

void FORM::release()
{
	this->prev->next = this->next;
	this->next->prev = this->prev;
	num_nodes--;
	this->next = headfree->next;
	this->prev = headfree;
	if (headfree->next != NULL)
		headfree->next->prev = this;
	headfree->next = this;
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
VARENTRY::VARENTRY(
	/* identifier of the variable */
	STBUCKET *id,
	/* graphical form for the variable */
	FORM *form,
	/* pointer to the next free variable */
	VARENTRY *nextvar)
{
	this->name = id;
	this->var = form;
	this->next = nextvar;
}

/* the following function allocate a new TERM entry */
TERM::TERM(
	/* pointer to the root form of the term */
	FORM *root_form,
	/* root port of the term */
	int root_ports,
	/* pointer to the free variables entries */
	/* of the term */
	VARENTRY *freevars)
{
	this->root_form = root_form;
	this->root_ports = root_ports;
	this->vars = freevars;
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
	/* new form to be created */

	FORM *variab = listvar->var;
	if (variab->kind != CONS1)
	{
		switch (variab->kind)
		{
		case TRIANGLE:
			variab->index = index;
			variab->nlevel[1]++;
			break;
		case CDR1:
		case CAR1:
		case TESTNIL1:
		case FAN:
			variab->index--;
			variab->nlevel[1]++;
			variab->nlevel[2]++;
			break;
		};
		listvar->next = addbrackets(index, listvar->next);
		return listvar;
	}

	FORM *bracket = new FORM(TRIANGLE, index);
	bracket->nlevel[1] = 1;
	connect(bracket, 1, variab, 0);
	return new VARENTRY(listvar->name,
						bracket,
						addbrackets(index, listvar->next));
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
	VARENTRY *res = NULL;

	VARENTRY *var = lookfor(l1->name, l2);
	if (var == NULL)
	{
		res = l1;
		res->next = share(index, l1->next, l2);
		return res;
	}

	FORM *fan = new FORM(FAN, index);
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

#include <list>

/* The following function searches for a variable inside a list. */
static VARENTRY *lookfor(
	/* pointer to the identifier to be found  */
	STBUCKET *id,
	/* pointer to the variable list to be scanned */
	VARENTRY *listvar)
{

	if (listvar == NULL)
		return NULL;

	if (listvar->name == id)
		return listvar;

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
	if (listvar == NULL)
	{
		return NULL;
	}

	if (id == listvar->name)
	{
		VARENTRY *temp = listvar->next;
		free(listvar);
		return temp;
	}

	listvar->next = remv(id, listvar->next);
	return listvar;
}

/* the following functions does the set substraction of two variable lists */
/* it runs in quadratic time, but who cares? */
static VARENTRY *remvp(
	VARLIST *vl,
	VARENTRY *listvar) /* pointer to the variable list to be scanned */
{
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
	if (listvar == NULL)
	{
		return;
	}

	FORM *formvar = listvar->var;
	FORM *formterm = listvar->name->curr_binding->root;
	if (formvar->kind == TRIANGLE)
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

/* The following function tries to merge two forms into a single one. */
/* If failing doing so, connects them normally        		      */

static void intelligent_connect(
	FORM *f1,
	int port,
	FORM *f2)
{
	int dep;
	FORM *newf;

	switch (f1->kind)
	{
	case FAN:
		switch (f2->kind)
		{
		case TRIANGLE:
			f1->nlevel[port] += f2->nlevel[1];
			inspect_connect(f1, port, f2->nform[1], f2->nport[1]);
			f2->release();
			break;
		case TESTNIL:
			f1->kind = TESTNIL1;
			f1->index = f2->index - f1->nlevel[port];
			if (port == 2)
			{
				dep = f1->nlevel[1];
				f1->nlevel[1] = f1->nlevel[2];
				f1->nlevel[2] = dep;
				inspect_connect(f1, 2, f1->nform[1], f1->nport[1]);
			}
			inspect_connect(f1, 1, f2->nform[1], f2->nport[1]);
			f2->release();
			break;
		case CAR:
			f1->kind = CAR1;
			f1->index = f2->index - f1->nlevel[port];
			if (port == 2)
			{
				dep = f1->nlevel[1];
				f1->nlevel[1] = f1->nlevel[2];
				f1->nlevel[2] = dep;
				inspect_connect(f1, 2, f1->nform[1], f1->nport[1]);
			}
			inspect_connect(f1, 1, f2->nform[1], f2->nport[1]);
			f2->release();
			break;
		case CDR:
			f1->kind = CDR1;
			f1->index = f2->index - f1->nlevel[port];
			if (port == 2)
			{
				dep = f1->nlevel[1];
				f1->nlevel[1] = f1->nlevel[2];
				f1->nlevel[2] = dep;
				inspect_connect(f1, 2, f1->nform[1], f1->nport[1]);
			}
			inspect_connect(f1, 1, f2->nform[1], f2->nport[1]);
			f2->release();
			break;
		default:
			connect(f1, port, f2, 0);
			break;
		}
		break;
	case TESTNIL1:
		switch (f2->kind)
		{
		case TRIANGLE:
			f1->nlevel[port] += f2->nlevel[1];
			inspect_connect(f1, port, f2->nform[1], f2->nport[1]);
			f2->release();
			break;
		default:
			connect(f1, port, f2, 0);
			break;
		}
		break;
	case CAR1:
		switch (f2->kind)
		{
		case TRIANGLE:
			f1->nlevel[port] += f2->nlevel[1];
			inspect_connect(f1, port, f2->nform[1], f2->nport[1]);
			f2->release();
			break;
		case CDR:
			if (port == 2)
			{
				f1->kind = CONS1;
				if (f1->nlevel[1] != 0)
				{
					newf = new FORM(TRIANGLE, f1->index - 1);
					newf->nlevel[1] = f1->nlevel[1];
					connect1(newf, 1, f1->nform[1], f1->nport[1]);
					connect(f1, 1, newf, 0);
				}
				if (f1->nlevel[2] != 0)
				{
					f2->kind = TRIANGLE;
					f2->nlevel[1] = f1->nlevel[2];
					f2->index = f1->index - 1;
					connect(f1, 2, f2, 0);
				}
				else
				{
					connect1(f1, 2, f2->nform[1], f2->nport[1]);
					f2->release();
				}
			}
			else
				connect(f1, port, f2, 0);
			break;
		case TESTNIL:
			if (port == 2)
			{
				f1->kind = TESTNIL1;
				f2->kind = CAR;
				dep = f2->index;
				f2->index = f1->index + f1->nlevel[1];
				f1->index = dep - f1->nlevel[2];
				dep = f1->nlevel[1];
				f1->nlevel[1] = f1->nlevel[2];
				f1->nlevel[2] = dep;
				newf = f1->nform[1];
				dep = f1->nport[1];
				connect1(f1, 1, f2->nform[1], f2->nport[1]);
				connect1(f2, 1, newf, dep);
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
		switch (f2->kind)
		{
		case TRIANGLE:
			f1->nlevel[port] += f2->nlevel[1];
			inspect_connect(f1, port, f2->nform[1], f2->nport[1]);
			f2->release();
			break;
		case CAR:
			if (port == 2)
			{
				f1->kind = CONS1;
				if (f1->nlevel[1] != 0)
				{
					newf = new FORM(TRIANGLE, f1->index - 1);
					newf->nlevel[1] = f1->nlevel[1];
					connect1(newf, 1, f1->nform[1], f1->nport[1]);
					connect(f1, 2, newf, 0);
				}
				else
					connect(f1, 2, f1->nform[1], f1->nport[1]);
				if (f1->nlevel[2] != 0)
				{
					f2->kind = TRIANGLE;
					f2->nlevel[1] = f1->nlevel[2];
					f2->index = f1->index - 1;
					connect(f1, 1, f2, 0);
				}
				else
				{
					connect1(f1, 1, f2->nform[1], f2->nport[1]);
					f2->release();
				}
			}
			else
				connect(f1, port, f2, 0);
			break;
		case TESTNIL:
			if (port == 2)
			{
				f1->kind = TESTNIL1;
				f2->kind = CDR;
				dep = f2->index;
				f2->index = f1->index + f1->nlevel[1];
				f1->index = dep - f1->nlevel[2];
				dep = f1->nlevel[1];
				f1->nlevel[1] = f1->nlevel[2];
				f1->nlevel[2] = dep;
				newf = f1->nform[1];
				dep = f1->nport[1];
				connect1(f1, 1, f2->nform[1], f2->nport[1]);
				connect1(f2, 1, newf, dep);
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

VARLIST *mergevarlist(
	VARLIST *l1,
	VARLIST *l2)
{
	if (!l1)
	{
		return l2;
	}
	VARLIST *p;
	for (p = l1; p->next; p = p->next)
	{
		if (l2->contains(p->id))
		{
			fprintf(stderr, "Pattern is not linear - expect garbage out!\n");
		}
	}
	if (l2->contains(p->id))
	{
		fprintf(stderr, "Pattern is not linear - expect garbage out!\n");
	}
	p->next = l2;
	return l1;
}

VARLIST *VARLIST::merge(VARLIST *other)
{
	return mergevarlist(this, other);
}

bool VARLIST::contains(BINDINGID *id)
{
	auto l = this;
	while (l)
	{
		if (id->id == l->id->id)
		{
			return true;
		}
		l = l->next;
	}
	return false;
}

VARLIST *makevarlist(
	STBUCKET *e,
	TERM *t)
{
	BINDINGID *bid = (BINDINGID *)malloc(sizeof(BINDINGID));
	bid->id = e;
	bid->form = t->root_form;
	VARLIST *vl = (VARLIST *)malloc(sizeof(VARLIST));
	vl->next = NULL;
	vl->id = bid;
	return vl;
}

// VARLIST1::VARLIST1(BINDINGID *id, TERM *t)
// {
// 	this->id = id;
// 	// this->list =
// 	// return nullptr;
// }

TERM *buildvoidterm(int level)
{
	FORM *newf = new FORM(TRIANGLE, level);
	newf->nlevel[1] = 0;
	return new TERM(newf, 0, NULL);
}

void PATTERN::release()
{
	VARLIST *vl, *vln;
	for (vl = this->var_list; vl; vl = vln)
	{
		vln = vl->next;
		free(vl);
	}
	free(this);
}
