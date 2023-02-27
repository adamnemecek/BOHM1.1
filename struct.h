#pragma once

#include <memory>

/* graphical form descriptor type */
struct FORM final
{
	/* name of the form */
	/* (FAN, ROOT, CROISSANT */
	/* BRACKET)  */
	char kind;
	/* Integer value for numeric */
	/* forms or indicator of safeness */
	/* for other forms  */
	int num_safe;
	/* index of the form */
	int index;
	/* numbers of the ports */
	/* where the three ports */
	/* of the form are connected to */
	/* (for CROISSANT and */
	/* BRACKET only the first two */
	/* fields are meaningful; for */
	/* ROOT only the first one is) */
	char nport[3];
	/* pointer to the forms */
	/* where the three ports */
	/* of the form are connected to */
	/* (for CROISSANT and */
	/* BRACKET only the first two */
	/* fields are meaningful; for */
	/* ROOT only the first one is) */
	FORM *nform[3];

	int nlevel[3];

	FORM *next;
	FORM *prev;

	FORM(int kind, int index);

	void release();

	FORM *inspect(int p);

	void reduce_term();
	void reduce_form();

	void del();
	FORM *copy(int p, int offset);

	FORM *lo_redex();

	void inspect_driver();
};

struct BINDINGENTRY;
struct PATTERN;

/* symbol table bucket type */
struct STBUCKET
{
	/* identifier */
	char *id;

	/* token associated with */
	/* the identifier (it can */
	/* be either ID or a keyword */
	/* token) */
	int token;

	/* pointer to the current */
	/* binding entry for the */
	/* identifier */
	BINDINGENTRY *curr_binding;

	/* pointer to the bucket */
	/* for the next identifier */
	/* hashing in the same */
	/* linked list of buckets */
	STBUCKET *next_st_bucket;

	STBUCKET(
		const char *id,
		int token)
	{
		this->id = strdup(id);
		this->token = token;
		this->curr_binding = NULL;
		this->next_st_bucket = NULL;
	}
};

/* binding entry descriptor type */
struct BINDINGENTRY
{
	STBUCKET *st_bucket;
	/* pointer to the bucket */
	/* for the identifier */
	/* involved in the binding */
	FORM *root;
	/* pointer to the root form */
	/* (for global identifiers) */
	BINDINGENTRY *prev_id_binding;
	/* pointer to the entry for */
	/* the binding previously */
	/* enforced for the same */
	/* identifier */
	BINDINGENTRY *prev_local_binding;
	/* pointer to the entry for the */
	/* binding previously enforced */
	/* in the same local environment */
};

/* local environment entry descriptor type */
struct LOCALENVENTRY
{
	/* nesting depth associated */
	/* with the local environment */
	int nesting_depth;

	/* pointer to the entry for */
	/* the last binding enforced in */
	/* this environment */
	BINDINGENTRY *last_local_binding;

	/* pointer to the entry for the */
	/* previous local environment */
	LOCALENVENTRY *prev_local_env;
};

/* free variable descriptor type */
struct VARENTRY
{
	/* pointer to the st_bucket */
	/* for the variable */
	STBUCKET *name;

	/* pointer to the form */
	/* for the variable */
	FORM *var;

	/* pointer to the next free */
	/* variable in a term */
	VARENTRY *next;

	VARENTRY(
		STBUCKET *id,
		FORM *form,
		VARENTRY *nextvar);
};

/* term descriptor type */
struct TERM final
{
	/* pointer to the root form */
	/* of the term */
	FORM *root_form;

	/* number of the root port */
	/* of the term (0 for variables */
	/* and abstractions, 1 for */
	/* applications) */
	char root_ports;

	/* pointer to the list of free */
	/* variables in the term */
	VARENTRY *vars;

	TERM(
		FORM *root_form,
		int root_ports,
		VARENTRY *freevars);

	static TERM *true_(int level);
	static TERM *false_(int level);

	static TERM *and_(
		int level,
		TERM *arg1,
		TERM *arg2);

	static TERM *car(
		int level,
		TERM *arg);

	static TERM *minus(
		int level,
		TERM *arg1);

	static TERM *cdr(
		int level,
		TERM *arg);

	static TERM *list(
		int level,
		TERM *arg1,
		TERM *arg2);

	static TERM *list1(
		int level,
		TERM *arg1,
		TERM *arg2);

	static TERM *matterm(
		int level,
		TERM *arg1,
		TERM *arg2,
		int op);

	static TERM *not_(
		int level,
		TERM *arg);

	static TERM *or_(
		int level,
		TERM *arg1,
		TERM *arg2);

	static TERM *app(
		int level,
		TERM *fun,
		TERM *arg);

	static TERM *ifelse(
		int level,
		TERM *arg1,
		TERM *arg2,
		TERM *arg3);

	static TERM *int_(
		int level,
		long int value);

	static TERM *lambda(
		int level,
		STBUCKET *id,
		TERM *body);

	static TERM *let_in(
		int level,
		STBUCKET *id,
		TERM *arg1,
		TERM *arg2);

	static TERM *mu(
		int level,
		STBUCKET *id,
		TERM *bod);

	static TERM *plambda(
		int level,
		PATTERN *pattern,
		TERM *body);

	static TERM *nillist(int level);
	static TERM *testnil(
		int level,
		TERM *arg);

	static TERM *relop(
		int level,
		TERM *arg1,
		TERM *arg2,
		int relop);

	static TERM *var(
		int level,
		STBUCKET *id);

	static TERM *void_(int level);

	TERM *makebox(int level);

	FORM *close(int level);

	// static TERM *lambda(int level, PATTERN *pattern,
	// TERM *body);
};

struct BINDINGID
{
	STBUCKET *id;
	FORM *form;

	BINDINGID(STBUCKET *id, FORM *form)
	{
		this->id = id;
		this->form = form;
	}
};

struct VARLIST
{
	BINDINGID *id;
	VARLIST *next;

	VARLIST *merge(VARLIST *other);
	bool contains(BINDINGID *id);
};

// struct VARLIST1
// {
// 	BINDINGID *id;
// 	std::list<VARENTRY *> list;

// 	VARLIST1(BINDINGID *id, TERM *t);
// };

struct PATTERN final
{
	VARLIST *var_list;
	TERM *term;

	PATTERN(TERM *term, VARLIST *var_list)
	{
		this->term = term;
		this->var_list = var_list;
	}

	~PATTERN();
};

struct COPY_FORM
{
	FORM *src;
	FORM *dest;
	COPY_FORM *next;

	COPY_FORM(FORM *src, FORM *dest, COPY_FORM *next)
	{
		this->src = src;
		this->dest = dest;
		this->next = next;
	}
};

struct ELEM
{
	FORM *node;
	int num;
	ELEM *next;
};
