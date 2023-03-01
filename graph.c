//**************************************************************
// This module contains routines for generating the graph
// representation of lambda-terms.
// The translation which has been implemented is the one
// described in "linear Logic, Comonads, and Optimal
// Reductions", by A.Asperti, to appear in Fundamenta
// Informaticae.
// During the translation, each term is described by a record
// with following fields:
//  - root_form : the root form of the term;
//  - root_ports : the root (positive) port of the root form;
//  - vars:   a pointer to the list of free variables for the
//            term.
// Free variable of a same term are collected into a linked
// list. Each entry for a free variable consists of:
//  - a pointer to the next variable;
//  - a pointer to the last form associated to the variable.
//  - a pointer to the symbol table bucket associated with the
//    the variable.
// The support of the implementation is an interaction net.
// Each connection between forms of the interaction net has
// been implemented by a double pointer. This is probably
// a bit redundant and inefficient, but it greatly simplified
// debugging, as it allows an easy inspection of the resulting
// graph (see also the directive "inspect" of the language).
// The following functions are external
//  - buildvarterm():  it builds the graph corresponding to a
//                     a variable. If the variable has been
//                     globally defined by a let instruction,
//                     the corresponding graph is a (shared)
//                     instance of the corresponding term.
//  - buildplambdaterm(): it builds the graph corresponding to
//                        a lambda abstraction.
//  - buildappterm(): it builds the graph corresponding to an
//                    application
//  - buildletinterm(): it builds the graph corresponding to a
//                      let_in instruction (it is equivalent to
//                      a partially evaluated lambda redex)
//  - closeterm(): it adds a root node to a term
//  - connect(): it connects two graphical forms at two
//               specified ports.
//  - connect1():  it connects the port portf1 of form1 to the
// 		   port portf2 of form2 and vice versa if the
//		   form2 is not a NIL, INT, True or False form.
//  - int_connect(): it connects only the port portf1 of form1
//		    to the port portf2 of form2, becouse form2
//		    is a INT.
//  - bool_connect(): it connects only the port portf1 of form1
// 		      to the port portf2 of form2, because
//		      form2 is a NIL, True or False.
//  - myfree():  it is used to handle a free list containig
//               all forms removed from the graph.
//		 These forms will be used for future
//		 allocations.
//  -new Form(): it allocates a new graphical form.
// The functions buildvarterm(), buildplambdaterm(),
// buildappterm() and buildletinterm() etc. are called by the
// parser.
// The functions connect() andnew Form() are also used
// during reduction.
// The following functions are local to the module:
//  - allocate_var(): it allocates a new entry for a variable.
//  - new Term(): it allocates a new entry for a term.
//  - makebox(): it builds a box around a term.
//  - addbrackets(): it adds a sequence of brackets of
//                   specified index along a sequence of
//                   variables.
//  - share(): it shares the free variables of two terms, by
//             adding suitable FANS.
//  - lookfor(): it searches for a variable inside a list.
//  - remv(): it removes a variable from a list.
//**************************************************************

#include "bohm.h"

unsigned num_nodes, max_nodes;

unsigned length_list = 0;

static VarEntry *addbrackets(
	int index,
	VarEntry *listvar);

static VarEntry *share(
	int index,
	VarEntry *l1,
	VarEntry *l2);

static VarEntry *lookfor(
	StBucket *id,
	VarEntry *listvar);

static VarEntry *remv(
	StBucket *id,
	VarEntry *listvar);

static VarEntry *remvp(
	VarList *vl,
	VarEntry *listvar);

// static VarEntry *allocate_var(
// 	StBucket *id,
// 	Form *form,
// 	VarEntry *nextvar);

static void closeglobalvars(VarEntry *listvar);

// The following function creates the graph representation of
// a variable
Term *Term::var(
	int level,
	StBucket *id)
{
	// pointer to the new form to be created
	Form *newf = new Form(TRIANGLE, level);
	newf->nlevel[1] = -1;

	// pointer to the new free variable entry
	VarEntry *newvar = new VarEntry(id, newf, NULL);
	return new Term(newf, 1, newvar);
}

// The following function creates the graph representation of
// a true constant
Term *Term::true_(int level)
{
	return new Term(NULL, T, NULL);
}

// The following function creates the graph representation of
// a false constant
Term *Term::false_(int level)
{
	return new Term(NULL, F, NULL);
}

// The following function creates the graph representation of
// a numerical constant
Term *Term::int_(
	int level,
	long int value)
{
	return new Term((Form *)value, INT, NULL);
}

// The following function creates the graph representation of a
// lambda-abstraction
Term *Term::lambda(
	int level,
	StBucket *id,
	Term *body)
{
	Term *t; // pointer to the new Term to be created

	VarEntry *boundvar = lookfor(id, body->vars);
	if (boundvar != NULL)
	{
		Form *newf1 = new Form(LAMBDA, level);
		Form *varform = boundvar->var;
		if (varform->kind == TRIANGLE && varform->nlevel[1] == 0)
		{
			connect(varform->nform[1],
					varform->nport[1],
					newf1, 2);
			varform->release();
		}
		else
		{
			connect(varform, 0, newf1, 2);
		}
		newf1->connect(1, body);
		// newf1->connect1(1, body);
		t = new Term(newf1, 0, remv(id, body->vars));
	}
	else
	{
		Form *newf1 = new Form(LAMBDAUNB, level);
		t = new Term(newf1, 0, body->vars);
		newf1->connect1(1, body);
	}
	free(body);
	return t;
}

Term *Term::plambda(
	int level,
	Pattern *pattern,
	Term *body)
{
	Term *t; // pointer to the new Term to be created
	VarList *vp;
	Form *newf1;
	Form *newf2;		// pointers to the new forms to be created
	VarEntry *boundvar; // pointer to the entry for the bound variable
	Form *varform;		// pointer to the bound variable form
	bool boundp;

	for (vp = pattern->var_list, boundp = false; vp; vp = vp->next)
		if (lookfor(vp->id->id, body->vars))
		{
			boundp = true;
			break;
		}

	if (boundp)
	{
		newf1 = new Form(LAMBDA, level);
		connect(pattern->term->root_form, 0, newf1, 2);
		for (vp = pattern->var_list; vp != NULL; vp = vp->next)
		{
			if ((boundvar = lookfor(vp->id->id, body->vars)) == 0)
			{
				newf2 = new Form(ERASE, level);
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
		} // for
		t = new Term(newf1, 0, remvp(pattern->var_list, body->vars));
		connect(newf1, 1, body->root_form, body->root_ports);
	}
	else
	{
		// apparent memory leak, but there's the destroyer
		newf1 = new Form(LAMBDAUNB, level);
		t = new Term(newf1, 0, body->vars);
		newf1->connect1(1, body);
	}
	free(body);
	return t;
}

// The following function creates the graph representation of a
// recursive definition.
Term *Term::mu(
	int level,
	StBucket *id,
	Term *body)
{
	Term *t;
	// pointer to the new Term to be created
	Term *temp;
	Form *newf1; // pointer to the new form to be created

	Form *varform;
	// pointer to the bound variable form

	// pointer to the entry for the bound variable
	VarEntry *boundvar = lookfor(id, body->vars);
	if (boundvar != NULL)
	{
		newf1 = new Form(FAN, level);
		newf1->nlevel[1] = -1;
		newf1->nlevel[2] = 1;
		varform = boundvar->var;

		if (varform->kind == TRIANGLE &&
			varform->nlevel[1] == 0)
		{
			connect(varform->nform[1],
					varform->nport[1],
					newf1, 2);
			varform->release();
		}
		else
		{
			newf1->intelligent_connect(2, varform);
		}
		temp = new Term(newf1, 1, remv(id, body->vars));
		t = temp->makebox(level);
		newf1->connect(0, body);
	}
	else
	{
		newf1 = new Form(TRIANGLE, level);
		newf1->nlevel[1] = -1;
		newf1->connect1(0, body);

		temp = new Term(newf1, 1, body->vars);
		t = temp->makebox(level);
	}
	free(body);
	return t;
}

// The following function creates the graph representation of
// an application
Term *Term::app(
	int level,
	Term *fun,
	Term *arg)
{
	Term *temp = arg->makebox(level);
	// free variables of the application
	Form *newf = new Form(APP, level);

	newf->connect1(0, fun);
	newf->connect1(2, temp);

	VarEntry *newvars = share(level, fun->vars, temp->vars);

	Term *t = new Term(newf, 1, newvars);
	free(arg);
	free(fun);
	return t;
}

// The following function creates the graph representation of
// an if_then_else term
Term *Term::ifelse(
	int level,
	Term *arg1,
	Term *arg2,
	Term *arg3)
{
	// free variables of the application
	Form *newf = new Form(IFELSE, level);
	Form *newf1 = new Form(CONS, level);
	// pointers to the new forms

	newf->connect1(0, arg1);
	connect(newf, 2, newf1, 0);

	newf1->connect1(1, arg2);
	newf1->connect1(2, arg3);

	VarEntry *tempvars = share(level, arg2->vars, arg3->vars);
	VarEntry *newvars = share(level, tempvars, arg1->vars);

	// pointer to the term to be created
	Term *t = new Term(newf, 1, newvars);
	free(arg1);
	free(arg2);
	free(arg3);
	return t;
}

// The following function creates the graph representation of
// a let_in expression
Term *Term::let_in(
	int level,
	StBucket *id,
	Term *arg1,
	Term *arg2)
{
	Term *temp = Term::lambda(level, id, arg2);
	return Term::app(level, temp, arg1);
}

// The following function creates the graph representation of
// a boolean and-expression
Term *Term::and_(
	int level,
	Term *arg1,
	Term *arg2)
{
	// pointer to the new form to be created
	Form *newf = new Form(AND, level);

	newf->connect1(0, arg1);
	newf->connect1(2, arg2);

	// free variables of the application
	VarEntry *newvars = share(level, arg1->vars, arg2->vars);

	// pointer to the term to be created
	Term *t = new Term(newf, 1, newvars);
	free(arg1);
	free(arg2);
	return t;
}

// The following function creates the graph representation of
// a boolean or-expression
Term *Term::or_(
	int level,
	Term *arg1,
	Term *arg2)
{
	// pointer to the new form to be created
	Form *newf = new Form(OR, level);

	newf->connect1(0, arg1);
	newf->connect1(2, arg2);

	// free variables of the application
	VarEntry *newvars = share(level, arg1->vars, arg2->vars);

	// pointer to the term to be created
	Term *t = new Term(newf, 1, newvars);
	free(arg1);
	free(arg2);
	return t;
}

// The following function creates the graph representation of
// a boolean not-expression
Term *Term::not_(
	int level,
	Term *arg)
{
	// pointer to the new form to be created
	Form *newf = new Form(NOT, level);

	newf->connect1(0, arg);

	// pointer to the term to be created
	Term *t = new Term(newf, 1, arg->vars);
	free(arg);
	return t;
}

// The following function creates the graph representation of
// a mat-expression
Term *Term::matterm(
	int level,
	Term *arg1,
	Term *arg2,
	int op)
{
	// pointer to the term to be created
	Term *t;

	// pointer to the new form to be created
	Form *newf = new Form(op, level);
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
		newf->connect1(0, arg2);
		t = new Term(newf, 1, arg2->vars);
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
				newf->nform[2] = (Form *)-(long int)newf->nform[2];
				break;
			case PROD:
				newf->kind = PROD1;
				break;
			}
			newf->connect1(0, arg1);

			t = new Term(newf, 1, arg1->vars);
		}
		else
		{
			newf->connect1(0, arg1);
			newf->connect1(2, arg2);
			// free variables of the application

			VarEntry *newvars = share(level, arg1->vars, arg2->vars);
			t = new Term(newf, 1, newvars);
		}
	}
	free(arg1);
	free(arg2);
	return t;
}

// The following function creates the graph representation of
// a unary minus-expression
Term *Term::minus(
	int level,
	Term *arg1)
{
	// pointer to the term to be created
	if (arg1->root_ports == INT)
	{
		arg1->root_form = (Form *)(-(long int)arg1->root_form);
		return arg1;
	}
	// pointer to the new form to be created
	Form *newf = new Form(SUB1, level);
	newf->num_safe = 0;
	newf->connect1(0, arg1);
	Term *t = new Term(newf, 1, arg1->vars);
	free(arg1);
	return t;
}

// The following function creates the graph representation of
// a relop-expression
Term *Term::relop(
	int level,
	Term *arg1,
	Term *arg2,
	int relop)
{
	// pointer to the term to be created
	Term *t;

	// pointer to the new form to be created
	Form *newf = new Form(relop, level);

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
		newf->connect1(0, arg2);
		t = new Term(newf, 1, arg2->vars);
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
			newf->connect1(0, arg1);
			t = new Term(newf, 1, arg1->vars);
		}
		else
		{
			newf->connect1(0, arg1);
			newf->connect1(2, arg2);
			// free variables of the application
			VarEntry *newvars = share(level, arg1->vars, arg2->vars);
			t = new Term(newf, 1, newvars);
		}
	}
	free(arg1);
	free(arg2);
	return t;
}

Term *Term::nillist(int level)
{
	return new Term(NULL, NIL, NULL);
}

Term *Term::list(
	int level,
	Term *arg1,
	Term *arg2)
{
	Term *t; // pointer to the term to be created
	// pointer to the new form to be created

	Form *newf1 = new Form(CONS, level);

	newf1->connect1(1, arg1);

	if (arg2 != NULL)
	{
		newf1->connect1(2, arg2);
		VarEntry *newvars = share(level, arg1->vars, arg2->vars);
		t = new Term(newf1, 0, newvars);
	}
	else
	{
		newf1->bool_connect(2, NIL);
		t = new Term(newf1, 0, arg1->vars);
	}

	free(arg1);
	free(arg2);
	return t;
}

Term *Term::list1(
	int level,
	Term *arg1,
	Term *arg2)
{
	Term *t; // pointer to the term to be created
	// pointer to the new form to be created

	Form *newf1 = new Form(CONS1, level);
	newf1->connect1(1, arg1);

	if (arg2 != NULL)
	{
		newf1->connect1(2, arg2);

		VarEntry *newvars = share(level, arg1->vars, arg2->vars);
		t = new Term(newf1, 0, newvars);
	}
	else
	{
		newf1->bool_connect(2, NIL);
		t = new Term(newf1, 0, arg1->vars);
	}

	free(arg1);
	free(arg2);
	return t;
}

Term *Term::car(
	int level,
	Term *arg)
{
	// pointer to the new form to be created
	Form *newf = new Form(CAR, level);
	newf->connect1(0, arg);
	Term *t = new Term(newf, 1, arg->vars);
	free(arg);
	return t;
}

Term *Term::cdr(
	int level,
	Term *arg)
{
	// pointer to the new form to be created
	Form *newf = new Form(CDR, level);
	newf->connect1(0, arg);
	Term *t = new Term(newf, 1, arg->vars);
	free(arg);
	return t;
}

Term *Term::testnil(
	int level,
	Term *arg)
{
	// pointer to the new form to be created
	Form *newf = new Form(TESTNIL, level);
	newf->connect1(0, arg);
	Term *t = new Term(newf, 1, arg->vars);
	free(arg);
	return t;
}

// the following function adds a root node to a term
Form *Term::close(
	int level)
{
	Form *newroot = new Form(ROOT, 0);

	newroot->connect1(0, this);

	if (level == 1)
	{
		this->vars = addbrackets(0, this->vars);
	}
	closeglobalvars(this->vars);
	return newroot;
}

// the following function allocate a new graphical form
// and initialize the name and index fields
Form::Form(
	// name of the form
	int kind,
	// index of the form
	int index)
{

	*this = *destroyer.alloc();
	num_nodes++;
	if (num_nodes > max_nodes)
	{
		max_nodes = num_nodes;
	}
	this->kind = kind;
	this->index = index;
	this->nform[0] = NULL;
	this->nform[1] = NULL;
	this->nform[2] = NULL;
	this->num_safe = true; // initially, all operators are safe
						   // return form;
}

// the following function adds a graphical form to deallocate
// in a list of free forms (i.e a free-list of forms)

void Form::release()
{
	this->prev->next = this->next;
	this->next->prev = this->prev;
	num_nodes--;
	destroyer.release(this);
}

// the following function connects together the port portf1 of
// form1 to the port portf2 of form2
void connect(
	Form *form1,
	int portf1,
	Form *form2,
	int portf2)
{
	form1->nport[portf1] = portf2;
	form1->nform[portf1] = form2;
	form2->nport[portf2] = portf1;
	form2->nform[portf2] = form1;
}

// the following function connects the port portf1 of form1 to
// the port portf2 of form2 and vice versa if the form2 is not
// a NIL, INT, True or False form.
void connect1(
	Form *form1,
	int portf1,
	Form *form2,
	int portf2)
{
	form1->nport[portf1] = portf2;
	form1->nform[portf1] = form2;
	if (is_connectable(portf2))
	{
		form2->nport[portf2] = portf1;
		form2->nform[portf2] = form1;
	}
}

void PORT::connect1(PORT other)
{
	::connect1(form, port, other.form, other.port);
}

void Form::connect1(
	int portf1,
	Term *term)
{
	::connect1(
		this,
		portf1,
		term->root_form, term->root_ports);
}

void Form::connect(int port, Term *term)
{
	::connect(this, port, term->root_form, term->root_ports);
}

void Form::connect(int port, PORT p)
{
	::connect(this, port, p.form, p.port);
}

void Form::connect1(int port, PORT p)
{
	::connect1(this, port, p.form, p.port);
}

// the following function connects only the port portf1 of
// form1 to the port portf2 of form2, because form2 is a INT
void int_connect(
	Form *form1,
	int portf1,
	Form *form2,
	int portf2)
{
	form1->nport[portf1] = portf2;
	form1->nform[portf1] = form2;
}

void Form::binop(long int (&op)(long int, long int))
{
	int_connect(
		this->nform[1],
		this->nport[1],
		(Form *)op((long int)this->nform[2], (long int)this->nform[0]),
		INT);
}
//**************************************************************
// 6. Definitions of functions strictly local to the module.
//**************************************************************

// the following function allocate a new variable entry
VarEntry::VarEntry(
	// identifier of the variable
	StBucket *id,
	// graphical form for the variable
	Form *form,
	// pointer to the next free variable
	VarEntry *nextvar)
{
	this->name = id;
	this->var = form;
	this->next = nextvar;
}

// the following function allocate a new Term entry
Term::Term(
	// pointer to the root form of the term
	Form *root_form,
	// root port of the term
	int root_ports,
	// pointer to the free variables entries
	// of the term
	VarEntry *freevars)
{
	this->root_form = root_form;
	this->root_ports = root_ports;
	this->vars = freevars;
}

// the following function build a box around a term
Term *Term::makebox(int level)
{
	this->vars = addbrackets(level, this->vars);
	return this;
}

// the following function add a sequence of square brackets of
// given index at the free variables in listvar
static VarEntry *addbrackets(
	int index,
	VarEntry *listvar)
{
	if (listvar == NULL)
	{
		return NULL;
	}
	// resulting variable entry list
	// new form to be created

	Form *variab = listvar->var;
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

	Form *bracket = new Form(TRIANGLE, index);
	bracket->nlevel[1] = 1;
	connect(bracket, 1, variab, 0);
	return new VarEntry(listvar->name,
						bracket,
						addbrackets(index, listvar->next));
}

// The following function shares the free variables of
// two terms, by adding suitable FANS.
static VarEntry *share(
	int index,
	// pointers to the lists of variables to be shared
	VarEntry *l1,
	VarEntry *l2)

{
	if (l1 == NULL)
	{
		return l2;
	}
	VarEntry *res = NULL;

	VarEntry *var = lookfor(l1->name, l2);
	if (var == NULL)
	{
		res = l1;
		res->next = share(index, l1->next, l2);
		return res;
	}

	Form *fan = new Form(FAN, index);
	fan->nlevel[1] = 0;
	fan->nlevel[2] = 0;

	fan->intelligent_connect(1, l1->var);
	fan->intelligent_connect(2, var->var);

	res = l1;
	res->name = l1->name;
	res->var = fan;
	res->next = share(index, l1->next, remv(l1->name, l2));
	return res;
}

// The following function searches for a variable inside a list.
static VarEntry *lookfor(
	// pointer to the identifier to be found
	StBucket *id,
	// pointer to the variable list to be scanned
	VarEntry *listvar)
{

	if (listvar == NULL)
	{
		return NULL;
	}

	if (listvar->name == id)
	{
		return listvar;
	}

	return lookfor(id, listvar->next);
}

// the following function remove an identifier form a list
// of variables
static VarEntry *remv(
	StBucket *id,
	// pointer to the identifier to be removed
	VarEntry *listvar)
// pointer to the variable list to be scanned
{
	if (listvar == NULL)
	{
		return NULL;
	}

	if (id == listvar->name)
	{
		VarEntry *temp = listvar->next;
		free(listvar);
		return temp;
	}

	listvar->next = remv(id, listvar->next);
	return listvar;
}

// the following functions does the set substraction of two variable lists
// it runs in quadratic time, but who cares?
static VarEntry *remvp(
	VarList *vl,
	VarEntry *listvar) // pointer to the variable list to be scanned
{
	for (VarList *v = vl; v; v = v->next)
	{
		listvar = remv(v->id->id, listvar);
	}
	return listvar;
}

// The following function copies all the graph of the global
// definition.
static void closeglobalvars(
	VarEntry *listvar)
// pointer to the variable list to be scanned
{
	if (listvar == NULL)
	{
		return;
	}

	Form *formvar = listvar->var;
	Form *formterm = listvar->name->curr_binding->root;
	if (formvar->kind == TRIANGLE)
	{
		Form *newf = formterm->nform[0]->copy(
			formterm->nport[0],
			formvar->nlevel[1]);
		connect1(formvar->nform[1], formvar->nport[1],
				 newf, formterm->nport[0]);
	}
	else
	{
		Form *newf = formterm->nform[0]->copy(formterm->nport[0], 0);
		connect1(formvar, 0,
				 newf, formterm->nport[0]);
	}
	closeglobalvars(listvar->next);
}

// The following function tries to merge two forms into a single one.
// If failing doing so, connects them normally

void Form::intelligent_connect(
	int port,
	Form *f2)
{
	int dep;
	Form *newf;

	switch (this->kind)
	{
	case FAN:
		switch (f2->kind)
		{
		case TRIANGLE:
			this->nlevel[port] += f2->nlevel[1];
			this->inspect_connect(port, f2->port(1));
			f2->release();
			break;
		case TESTNIL:
		case CAR:
		case CDR:
			this->kind = unpropagate_kind(this->kind);
			this->index = f2->index - this->nlevel[port];
			if (port == 2)
			{
				dep = this->nlevel[1];
				this->nlevel[1] = this->nlevel[2];
				this->nlevel[2] = dep;
				this->inspect_connect(2, this->port(1));
			}
			this->inspect_connect(1, f2->port(1));
			f2->release();
			break;
		default:
			::connect(this, port, f2, 0);
			break;
		}
		break;
	case TESTNIL1:
		switch (f2->kind)
		{
		case TRIANGLE:
			this->nlevel[port] += f2->nlevel[1];
			this->inspect_connect(port, f2->port(1));
			f2->release();
			break;
		default:
			::connect(this, port, f2, 0);
			break;
		}
		break;
	case CAR1:
		switch (f2->kind)
		{
		case TRIANGLE:
			this->nlevel[port] += f2->nlevel[1];
			this->inspect_connect(port, f2->port(1));
			f2->release();
			break;
		case CDR:
			if (port == 2)
			{
				this->kind = CONS1;
				if (this->nlevel[1] != 0)
				{
					newf = new Form(TRIANGLE, this->index - 1);
					newf->nlevel[1] = this->nlevel[1];
					newf->connect1(1, this->port(1));
					::connect(this, 1, newf, 0);
				}
				if (this->nlevel[2] != 0)
				{
					f2->kind = TRIANGLE;
					f2->nlevel[1] = this->nlevel[2];
					f2->index = this->index - 1;
					::connect(this, 2, f2, 0);
				}
				else
				{
					this->connect1(2, f2->port(1));
					f2->release();
				}
			}
			else
			{
				::connect(this, port, f2, 0);
			}
			break;
		case TESTNIL:
			if (port == 2)
			{
				this->kind = TESTNIL1;
				f2->kind = CAR;
				dep = f2->index;
				f2->index = this->index + this->nlevel[1];
				this->index = dep - this->nlevel[2];
				dep = this->nlevel[1];
				this->nlevel[1] = this->nlevel[2];
				this->nlevel[2] = dep;
				newf = this->nform[1];
				dep = this->nport[1];
				this->connect1(1, f2->port(1));
				::connect1(f2, 1, newf, dep);
				::connect(f2, 0, this, 2);
			}
			else
			{
				::connect(this, port, f2, 0);
			}
			break;
		default:
			::connect(this, port, f2, 0);
			break;
		}
		break;
	case CDR1:
		switch (f2->kind)
		{
		case TRIANGLE:
			this->nlevel[port] += f2->nlevel[1];
			this->inspect_connect(port, f2->port(1));
			f2->release();
			break;
		case CAR:
			if (port == 2)
			{
				this->kind = CONS1;
				if (this->nlevel[1] != 0)
				{
					newf = new Form(TRIANGLE, this->index - 1);
					newf->nlevel[1] = this->nlevel[1];
					newf->connect1(1, this->port(1));
					::connect(this, 2, newf, 0);
				}
				else
				{
					::connect(this, 2, this->nform[1], this->nport[1]);
				}
				if (this->nlevel[2] != 0)
				{
					f2->kind = TRIANGLE;
					f2->nlevel[1] = this->nlevel[2];
					f2->index = this->index - 1;
					::connect(this, 1, f2, 0);
				}
				else
				{
					this->connect1(1, f2->port(1));
					f2->release();
				}
			}
			else
			{
				::connect(this, port, f2, 0);
			}
			break;
		case TESTNIL:
			if (port == 2)
			{
				this->kind = TESTNIL1;
				f2->kind = CDR;
				dep = f2->index;
				f2->index = this->index + this->nlevel[1];
				this->index = dep - this->nlevel[2];
				dep = this->nlevel[1];
				this->nlevel[1] = this->nlevel[2];
				this->nlevel[2] = dep;
				newf = this->nform[1];
				dep = this->nport[1];
				newf->connect1(1, f2->port(2));
				::connect1(f2, 1, newf, dep);
				::connect(f2, 0, this, 2);
			}
			else
			{
				::connect(this, port, f2, 0);
			}
			break;
		default:
			::connect(this, port, f2, 0);
			break;
		}
		break;
	default:
		::connect(this, port, f2, 0);
		break;
	}
}

// The following function checks whether it's possible to apply
// function intelligent_connect. Otherwise applies a normal connect.
void Form::inspect_connect(
	int p1,
	PORT p)
{
	if (p.port == 0)
	{
		this->intelligent_connect(p1, p.form);
	}
	else
	{
		this->connect1(p1, p);
	}
}

VarList *mergevarlist(
	VarList *l1,
	VarList *l2)
{
	if (!l1)
	{
		return l2;
	}
	VarList *p;
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

VarList *VarList::merge(VarList *other)
{
	return mergevarlist(this, other);
}

bool VarList::contains(BindingID *id)
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

VarList *makevarlist(
	StBucket *e,
	Term *t)
{
	BindingID *bid = (BindingID *)malloc(sizeof(BindingID));
	bid->id = e;
	bid->form = t->root_form;
	VarList *vl = (VarList *)malloc(sizeof(VarList));
	vl->next = NULL;
	vl->id = bid;
	return vl;
}

// VARLIST1::VARLIST1(BindingID *id, Term *t)
// {
// 	this->id = id;
// 	// this->list =
// 	// return nullptr;
// }

Term *Term::void_(int level)
{
	Form *newf = new Form(TRIANGLE, level);
	newf->nlevel[1] = 0;
	return new Term(newf, 0, NULL);
}

Pattern::~Pattern()
{
	VarList *vln;
	for (VarList *vl = this->var_list; vl; vl = vln)
	{
		vln = vl->next;
		free(vl);
	}
	// free(this);
}
