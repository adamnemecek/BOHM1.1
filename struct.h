#pragma once

/* graphical form descriptor type */
struct FORM
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

	void uninit();
};

struct BINDINGENTRY;

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
	BINDINGENTRY *prev_id_binding,
		/* pointer to the entry for */
		/* the binding previously */
		/* enforced for the same */
		/* identifier */
		*prev_local_binding;
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

	VARENTRY(STBUCKET *id, FORM *form, VARENTRY *nextvar);
};

/* term descriptor type */
struct TERM
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

	// static TERM *lambda(int level, PATTERN *pattern,
	// TERM *body);
};

struct BINDINGID
{
	STBUCKET *id;
	FORM *form;
};

struct VARLIST
{
	BINDINGID *id;
	VARLIST *next;
};

struct PATTERN
{
	VARLIST *var_list;
	TERM *term;
};

struct COPY_FORM
{
	FORM *src;
	FORM *dest;
	COPY_FORM *next;
};

struct ELEM
{
	FORM *node;
	int num;
	ELEM *next;
};
