#pragma once

#include <memory>
struct Term;
struct Form;

struct Port
{
	Form *form;
	int port;

	void connect1(Port other);
};

#define BINOP(name, op)   \
	template <typename A> \
	A name(A a, A b)      \
	{                     \
		return a op b;    \
	};

BINOP(add, +);
BINOP(sub, -);
BINOP(mult, *);
BINOP(div, /);
BINOP(rem, %);

// graphical form descriptor type
struct Form final
{
	// name of the form
	// (FAN, ROOT, CROISSANT
	// BRACKET)
	char kind;
	// Integer value for numeric
	// forms or indicator of safeness
	// for other forms
	int num_safe;
	// index of the form
	int index;
	// numbers of the ports
	// where the three ports
	// of the form are connected to
	// (for CROISSANT and
	// BRACKET only the first two
	// fields are meaningful; for
	// ROOT only the first one is)
	char nport[3];
	//  pointer to the forms
	//  where the three ports
	//  of the form are connected to
	//  (for CROISSANT and
	//  BRACKET only the first two
	//  fields are meaningful; for
	//  ROOT only the first one is)
	Form *nform[3];

	// these are some sort of offset
	int nlevel[3];

	Form *next;
	Form *prev;

	Form(
		int kind,
		int index);

	void release();

	void stampa(
		int p,
		int card);

	Form *inspect(int p);

	void reduce_term();
	void reduce_form();

	void del();

	void save_aux(int p);

	void put_int(const int p);

	Port port(int i)
	{
		return Port{nform[i], nport[i]};
	}

	Form *copy(
		int p,
		int offset);

	Form *copy_aux(
		int p,
		int offset);

	// Form *copy(
	// 	Form *root,
	// 	int p,
	// 	int offset);

	Form *lo_redex();

	void rdbk();
	void rdbk_list(int port);
	void rdbk_1(int port);

	void inspect_driver();

	int hash();

	int present();
	void put_form();

	void save(
		char *name,
		char *id);

	void connect(int port, Term *term);
	void connect1(int port, Term *term);

	// the following function connects only the port portf1 of
	// form1 to the port portf2 of form2, because form2 is a INT,
	// NIL, True or False.
	void bool_connect(int port, int val)
	{
		this->nport[port] = val;
	}

	void intelligent_connect(int port, Form *f2);
	void binop(long int (&op)(long int, long int));

	// void selfconnect1(int from, int to);

	void connect(int port, Port p);
	void connect1(int port, Port p);
	void garbage();

	void int_connect(int port, Port p);

	void inspect_connect(
		int p1,
		Port p);

	static Form *or_(
		int level,
		Term *arg1,
		Term *arg2)
	{
		Form *newf = new Form(OR, level);

		newf->connect1(0, arg1);
		newf->connect1(2, arg2);
		return newf;
	}
};

struct BindingEntry;
struct Pattern;

// symbol table bucket type
struct StBucket final
{
	// identifier
	char *id;

	// token associated with
	// the identifier (it can
	// be either ID or a keyword
	// token)
	int token;

	// pointer to the current
	// binding entry for the
	// identifier
	BindingEntry *curr_binding;

	// pointer to the bucket
	// for the next identifier
	// hashing in the same
	// linked list of buckets
	StBucket *next_st_bucket;

	StBucket(
		const char *id,
		int token)
	{
		this->id = strdup(id);
		this->token = token;
		this->curr_binding = NULL;
		this->next_st_bucket = NULL;
	}

	void create_variable_binding(Form *form);
};

// binding entry descriptor type
struct BindingEntry final
{
	StBucket *st_bucket;
	// pointer to the bucket
	// for the identifier
	// involved in the binding
	Form *root;
	// pointer to the root form
	// (for global identifiers)
	BindingEntry *prev_id_binding;
	// pointer to the entry for
	// the binding previously
	// enforced for the same
	// identifier
	BindingEntry *prev_local_binding;
	// pointer to the entry for the
	// binding previously enforced
	// in the same local environment
};

// local environment entry descriptor type
struct LocalEnvEntry final
{
	// nesting depth associated
	// with the local environment
	int nesting_depth;

	// pointer to the entry for
	// the last binding enforced in
	// this environment
	BindingEntry *last_local_binding;

	// pointer to the entry for the
	// previous local environment
	LocalEnvEntry *prev_local_env;
};

// free variable descriptor type
struct VarEntry final
{
	// pointer to the st_bucket
	// for the variable
	StBucket *name;

	// pointer to the form
	// for the variable
	Form *var;

	// pointer to the next free
	// variable in a term
	VarEntry *next;

	VarEntry(
		StBucket *id,
		Form *form,
		VarEntry *nextvar);
};

// term descriptor type
struct Term final
{
	// pointer to the root form
	// of the term
	Form *root_form;

	// number of the root port
	// of the term (0 for variables
	// and abstractions, 1 for
	// applications)
	char root_ports;

	// pointer to the list of free
	// variables in the term
	VarEntry *vars;

	Term(
		Form *root_form,
		int root_ports,
		VarEntry *freevars);

	static Term *true_(int level);
	static Term *false_(int level);

	static Term *int_(
		int level,
		long int value);

	static Term *and_(
		int level,
		Term *arg1,
		Term *arg2);

	static Term *or_(
		int level,
		Term *arg1,
		Term *arg2);

	static Term *car(
		int level,
		Term *arg);

	static Term *cdr(
		int level,
		Term *arg);

	static Term *minus(
		int level,
		Term *arg1);

	static Term *list(
		int level,
		Term *arg1,
		Term *arg2);

	static Term *list1(
		int level,
		Term *arg1,
		Term *arg2);

	static Term *matterm(
		int level,
		Term *arg1,
		Term *arg2,
		int op);

	static Term *not_(
		int level,
		Term *arg);

	static Term *app(
		int level,
		Term *fun,
		Term *arg);

	static Term *ifelse(
		int level,
		Term *arg1,
		Term *arg2,
		Term *arg3);

	static Term *lambda(
		int level,
		StBucket *id,
		Term *body);

	static Term *let_in(
		int level,
		StBucket *id,
		Term *arg1,
		Term *arg2);

	static Term *mu(
		int level,
		StBucket *id,
		Term *bod);

	static Term *plambda(
		int level,
		Pattern *pattern,
		Term *body);

	static Term *nillist(int level);

	static Term *testnil(
		int level,
		Term *arg);

	static Term *relop(
		int level,
		Term *arg1,
		Term *arg2,
		int relop);

	static Term *var(
		int level,
		StBucket *id);

	static Term *void_(int level);

	Term *makebox(int level);

	Form *close(int level);
};

struct BindingID final
{
	StBucket *id;
	Form *form;

	BindingID(StBucket *id, Form *form)
	{
		this->id = id;
		this->form = form;
	}
};

struct VarList final
{
	BindingID *id;
	VarList *next;

	VarList *merge(VarList *other);
	bool contains(BindingID *id);
};

// struct VARLIST1
// {
// 	BindingID *id;
// 	std::list<VarEntry *> list;

// 	VARLIST1(BindingID *id, Term *t);
// };

struct Pattern final
{
	VarList *var_list;
	Term *term;

	Pattern(Term *term, VarList *var_list)
	{
		this->term = term;
		this->var_list = var_list;
	}

	~Pattern();
};

// this is a dependency of sorts
struct CopyForm final
{
	Form *src;
	Form *dest;
	CopyForm *next;

	CopyForm(Form *src, Form *dest, CopyForm *next)
	{
		this->src = src;
		this->dest = dest;
		this->next = next;
	}
};

struct Elem final
{
	Form *node;
	int num;
	Elem *next;

	Elem(
		Form *node,
		int num,
		Elem *next)
	{
		this->node = node;
		this->num = num;
		this->next = next;
	}

	void eindex();
};
