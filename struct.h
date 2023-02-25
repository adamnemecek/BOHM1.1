#pragma once

/* symbol table bucket type */
typedef struct st_bucket
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
	struct binding_entry *curr_binding;

	/* pointer to the bucket */
	/* for the next identifier */
	/* hashing in the same */
	/* linked list of buckets */
	struct st_bucket *next_st_bucket;
} STBUCKET;

/* local environment entry descriptor type */
typedef struct local_env_entry
{
	/* nesting depth associated */
	/* with the local environment */
	int nesting_depth;

	/* pointer to the entry for */
	/* the last binding enforced in */
	/* this environment */
	struct binding_entry *last_local_binding;

	/* pointer to the entry for the */
	/* previous local environment */
	struct local_env_entry *prev_local_env;
} LOCALENVENTRY;

/* binding entry descriptor type */
typedef struct binding_entry
{
	struct st_bucket *st_bucket;
	/* pointer to the bucket */
	/* for the identifier */
	/* involved in the binding */
	struct form *root;
	/* pointer to the root form */
	/* (for global identifiers) */
	struct binding_entry *prev_id_binding,
		/* pointer to the entry for */
		/* the binding previously */
		/* enforced for the same */
		/* identifier */
		*prev_local_binding;
	/* pointer to the entry for the */
	/* binding previously enforced */
	/* in the same local environment */
} BINDINGENTRY;

/* graphical form descriptor type */
typedef struct form
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
	struct form *nform[3];

	int nlevel[3];

	struct form *next;
	struct form *prev;

} FORM;

/* free variable descriptor type */
typedef struct varentry
{
	/* pointer to the st_bucket */
	/* for the variable */
	struct st_bucket *name;

	/* pointer to the form */
	/* for the variable */
	struct form *var;

	/* pointer to the next free */
	/* variable in a term */
	struct varentry *next;
} VARENTRY;

/* term descriptor type */
typedef struct term
{
	/* pointer to the root form */
	/* of the term */
	struct form *root_form;

	/* number of the root port */
	/* of the term (0 for variables */
	/* and abstractions, 1 for */
	/* applications) */
	char root_ports;

	/* pointer to the list of free */
	/* variables in the term */
	struct varentry *vars;
} TERM;

typedef struct binding_id
{
	struct st_bucket *id;
	struct form *form;
} BINDINGID;

typedef struct var_list
{
	struct binding_id *id;
	struct var_list *next;
} VARLIST;

typedef struct pattern
{
	struct var_list *var_list;
	struct term *term;
} PATTERN;

typedef struct copy_form
{
	FORM *src;
	FORM *dest;
	struct copy_form *next;
} COPY_FORM;

typedef struct elem
{
	FORM *node;
	int num;
	struct elem *next;
} ELEM;
