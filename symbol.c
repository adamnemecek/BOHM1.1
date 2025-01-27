//**************************************************************
// This module supplies routines for symbol table handling.
// The symbol table consists of two parts: a dictionary and a
// scope stack.
// The dictionary is organized in the following way:
// - there is a bucket for each distinct identifier; the bucket
//   contains:
//   - a field containing a pointer to the sequence of
//     characters that spell out the identifier;
//   - a field indicating the token associated with the
//     identifier (remember that keywords are treated as
//     identifiers);
//   - a field containing a pointer to the entry for the
//     current binding in which the identifier is involved;
//   - a field containing a pointer to the next bucket;
// - buckets are collected into linked lists; each list is
//   self-organizing, i.e. the first bucket of the list is
//   associated with the last identifier inserted into or
//   looked for.
// - the first bucket of each list is pointed to by a pointer
//   stored in a hash table that contains 211 pointers; the
//   hash function that has been used was studied by P. J.
//   Weinberger (see A. V. Aho-R. Sethi-J. D. Ullman/
//   "Compilers"/pages 433-438).
// The scope stack has been meant to be easily extendible to
//   the case of multibinding, and is organized in the
//   following way:
// - there is an entry for each distinct local environment; the
//   entry contains:
//   - a field containing the nesting depth associated with the
//     local environment of the function (not used);
//   - a field containing a pointer to the entry for the last
//     binding created in this local environment (in the case
//     of pure lambda calculus this entry is unique);
//   - a field containing a pointer to the entry for the
//     previous local environment;
// - local environment entries are collected into a linked list
//   implementing a stack; the first two entries pushed onto
//   the scope stack concern the external environment (set of
//   bindings involving library procedures: it is empty,
//   for the moment) and the global environment.
// The two parts of the symbol table interact as follows:
// - there is an entry for each distinct binding; the entry
//   contains:
//   - a field containing a pointer to the bucket for the
//     identifier involved in the binding;
//   - a field containing a pointer to the previous binding
//     for the identifier involved in this binding;
//   - a field containing a pointer to the previous binding
//     created in the local environment containing this
//     binding;
// - binding entries are collected into linked lists; each
//   binding entry lies in:
//   - the binding entry list for the identifier involved in
//     the binding (this list is not doubly linked thanks to
//     the presence, in each binding entry, of the field
//     containing a pointer to the bucket for the identifier
//     involved in the binding);
//   - the binding entry list for the local environment in
//     which the binding has been created.
// - init_symbol_table(): it initializes the symbol table by
//			  inserting keywords into the
//			  dictionary and the external and
//			  global environments into the scope
//			  stack;
// - search_bucket(): it searches the symbol table for the
//		      bucket containing a given identifier, and
//		      inserts it if it is not present;
// - push_local_env(): it pushes a new local environment entry
//		       onto the scope stack;
// - pop_local_env(): it pops a local environment entry off the
//		      scope stack;
// - create_variable_binding(): it creates an entry for
//				a variable declaration
// - allocate_bucket(): it allocates a bucket for an
//			identifier;
// - move_bucket(): it moves a bucket to the head of the list
//		    in which it lies;
// - hash_pjw(): it computes the hash function;
// - allocate_local_env_entry(): it allocates a local
//				 environment entry.
//**************************************************************

//**************************************************************
// 1. Inclusion of header files.
//**************************************************************

#include "bohm.h"
#include "y.tab.h"

//**************************************************************
// 2. Inclusion of declarations that are being imported.
//**************************************************************

//**************************************************************
// 3. Definitions of variables to be exported.
//**************************************************************

//**************************************************************
// 4. Definitions strictly local to the module.
//**************************************************************

// constants concerning keywords
#define KEYWORDNUM 28
#define FIRSTKEYWORD 400

// constants concerning the symbol table
#define DICTSIZE 211
#define HASH1 4
#define HASH2 0xf0000000
#define HASH3 24

// constants concerning scope analysis
#define NONESTING -2

// static LocalEnvEntry *curr_local_env;
// pointer to the entry for the
// current local environment

// static StBucket *dictionary[DICTSIZE] = {0};
// keywords
const char *keywords[] =
	{
		"let",
		"in",
		"inspect",
		"quit",
		"load",
		"rec",
		"true",
		"false",
		"if",
		"then",
		"else",
		"and",
		"or",
		"not",
		"div",
		"mod",
		"cons",
		"head",
		"tail",
		"isnil",
		"def",
		"share",
		"nil",
		"garbage",
		"option",
		"info",
		"save",
		"travel",
};

// The following function implements Weinberger's hash function.
static int hash_pjw(const char *id)
{
	unsigned h;

	for (h = 0; *id != '\0'; id++)
	{
		h = (h << HASH1) + (*id);
		unsigned g = h & HASH2;
		if (g)
		{
			h = h ^ (g >> HASH3) ^ g;
		}
	}
	return h % DICTSIZE;
}

SymbolTable::SymbolTable()
{
	curr_local_env = NULL;
	curr_nesting_depth = NONESTING;

	reset();

	for (int i = 0; i < KEYWORDNUM; i++)
	{
		StBucket *st = search_bucket(keywords[i]);
		st->token = FIRSTKEYWORD + i;
	}

	push_local_env();
	push_local_env();
}

// The following function allocates a local environment entry.
void SymbolTable::allocate_local_env_entry()
{
	// pointer to the entry to
	// be allocated
	LocalEnvEntry *le = (LocalEnvEntry *)malloc_da(sizeof(LocalEnvEntry));
	le->nesting_depth = curr_nesting_depth;
	le->last_local_binding = NULL;
	le->prev_local_env = curr_local_env;
	curr_local_env = le;
}

// The following function allocates a bucket for an identifier.
StBucket *SymbolTable::allocate_bucket(const char *id)
{
	StBucket *st = (StBucket *)malloc_da(sizeof(StBucket));
	st->id = strdup(id);
	st->token = ID;
	st->curr_binding = NULL;
	st->next_st_bucket = NULL;
	return st;
}

// The following function moves a bucket to the head of the
// list in which it lies.
void SymbolTable::move_bucket(
	// pointer to the bucket to
	// be moved
	StBucket *st,
	// index corresponding to
	// the list in which the
	// bucket lies
	int dict_index)
{
	st->next_st_bucket = dictionary[dict_index];
	dictionary[dict_index] = st;
}

// The following function pops a local environment entry off
// the scope stack.
void SymbolTable::pop_local_env()
{
	LocalEnvEntry *le = curr_local_env;

	// remove all the entries for bindings created in the
	// local environment
	while (le->last_local_binding != NULL)
	{
		BindingEntry *b = le->last_local_binding;
		b->st_bucket->curr_binding = b->prev_id_binding;
		le->last_local_binding = b->prev_local_binding;
		free(b);
	}

	curr_local_env = le->prev_local_env;
	free(le);
	curr_nesting_depth--;
}

// The following function pushes a new local environment entry onto
// the scope stack.
void SymbolTable::push_local_env()
{
	curr_nesting_depth++;
	allocate_local_env_entry();
}

void SymbolTable::reset()
{
	for (int i = 0; i < DICTSIZE; i++)
	{
		dictionary[i] = NULL;
	}
}

StBucket *SymbolTable::search_bucket(const char *id)
{
	StBucket *curr;
	StBucket *st;

	// turn the identifier into lower case
	// to_lower_s(id);

	// apply the hash function
	int dict_index = hash_pjw(id);

	// scan the bucket list indicated by the hash function
	StBucket *prev = curr = dictionary[dict_index];
	while (curr != NULL && strcmp(id, curr->id) != 0)
	{
		prev = curr;
		curr = curr->next_st_bucket;
	}

	// the identifier is not in the list
	if (curr == NULL)
	{
		st = new StBucket(id, ID);
		move_bucket(st, dict_index);
		return st;
	}
	// the identifier is already in the list
	st = curr;
	if (prev != curr)
	// the identifier is not in the first position
	{
		prev->next_st_bucket = curr->next_st_bucket;
		move_bucket(curr, dict_index);
	}
	return st;
}

// pointers to bucket lists

// static int curr_nesting_depth;
// current nesting depth

// static int hash_pjw(const char *id);
// static void allocate_local_env_entry(void);

// static void move_bucket(
// 	StBucket *st,
// 	int dict_index);

// static StBucket *allocate_bucket(const char *id);

// The following function turns a given string into a lower case one.
static void to_lower_s(char *s)
{
	for (; *s != '\0'; s++)
	{
		if ('A' <= *s && *s <= 'Z')
		{
			*s += 'a' - 'A';
		}
	}
}

//**************************************************************
// 5. Definitions of functions to be exported.
//**************************************************************

// The following function initializes the symbol table by inserting
// P keywords into the dictionary and the external and global
// environments into the scope stack.
// void init_symbol_table(void)
// {
// 	// initialize the dictionary
// 	for (int i = 0; i < DICTSIZE; i++)
// 	{
// 		dictionary[i] = NULL;
// 	}

// 	// insert P keywords into the appropriate bucket lists
// 	for (int i = 0; i < KEYWORDNUM; i++)
// 	{
// 		StBucket *st = search_bucket(keywords[i]);
// 		st->token = FIRSTKEYWORD + i;
// 	}

// 	// initialize the scope stack
// 	curr_local_env = NULL;
// 	curr_nesting_depth = NONESTING;

// 	push_local_env();
// 	push_local_env();
// }

// The following function searches the symbol table for an identifier
// and inserts it if it is not present; the function returns the
// pointer to the bucket containing information associated with the
// given identifier. The bucket associated with the given identifier
// becomes the first one in its list.

// StBucket *search_bucket(
// 	// identifier
// 	const char *id)
// {
// 	StBucket *curr;
// 	StBucket *st;

// 	// turn the identifier into lower case
// 	// to_lower_s(id);

// 	// apply the hash function
// 	int dict_index = hash_pjw(id);

// 	// scan the bucket list indicated by the hash function
// 	StBucket *prev = curr = dictionary[dict_index];
// 	while (curr != NULL && strcmp(id, curr->id) != 0)
// 	{
// 		prev = curr;
// 		curr = curr->next_st_bucket;
// 	}

// 	// the identifier is not in the list
// 	if (curr == NULL)
// 	{
// 		st = new StBucket(id, ID);
// 		move_bucket(st, dict_index);
// 		return st;
// 	}
// 	// the identifier is already in the list
// 	st = curr;
// 	if (prev != curr)
// 	// the identifier is not in the first position
// 	{
// 		prev->next_st_bucket = curr->next_st_bucket;
// 		move_bucket(curr, dict_index);
// 	}
// 	return st;
// }

// StBucket *SymbolTable::find(const char *id)
// {
// 	return nullptr;
// }

// The following function pushes a new local environment entry onto
// the scope stack.
// void push_local_env()
// {
// 	curr_nesting_depth++;
// 	allocate_local_env_entry();
// }

// The following function pops a local environment entry off
// the scope stack.
// void pop_local_env()
// {
// 	LocalEnvEntry *le = curr_local_env;

// 	// remove all the entries for bindings created in the
// 	// local environment
// 	while (le->last_local_binding != NULL)
// 	{
// 		BindingEntry *b = le->last_local_binding;
// 		b->st_bucket->curr_binding = b->prev_id_binding;
// 		le->last_local_binding = b->prev_local_binding;
// 		free(b);
// 	}

// 	curr_local_env = le->prev_local_env;
// 	free(le);
// 	curr_nesting_depth--;
// }

// The following function creates entries for a variable binding
// pointer to the bucket for the
// identifier which is to be bound
// to a procedure

void StBucket::create_variable_binding(

	// pointer to the rootform of the
	// term associated with the identifier
	// (for global declarations only)

	Form *rootform)
{
	BindingEntry *b = (BindingEntry *)malloc_da(sizeof(BindingEntry));
	b->st_bucket = this;
	b->root = rootform;
	b->prev_id_binding = this->curr_binding;
	this->curr_binding = b;
	b->prev_local_binding = st.curr_local_env->last_local_binding;
	st.curr_local_env->last_local_binding = b;
}

//**************************************************************
// 6. Definitions of functions strictly local to the module.
//**************************************************************

// The following function allocates a bucket for an identifier.
// static StBucket *allocate_bucket(const char *id)
// {
// 	StBucket *st = (StBucket *)malloc_da(sizeof(StBucket));
// 	st->id = strdup(id);
// 	st->token = ID;
// 	st->curr_binding = NULL;
// 	st->next_st_bucket = NULL;
// 	return st;
// }

// The following function moves a bucket to the head of the
// list in which it lies.
// static void move_bucket(
// 	// pointer to the bucket to
// 	// be moved
// 	StBucket *st,
// 	// index corresponding to
// 	// the list in which the
// 	// bucket lies
// 	int dict_index)
// {
// 	st->next_st_bucket = dictionary[dict_index];
// 	dictionary[dict_index] = st;
// }

// static void allocate_local_env_entry(void)
// {
// 	// pointer to the entry to
// 	// be allocated
// 	LocalEnvEntry *le = (LocalEnvEntry *)malloc_da(sizeof(LocalEnvEntry));
// 	le->nesting_depth = curr_nesting_depth;
// 	le->last_local_binding = NULL;
// 	le->prev_local_env = curr_local_env;
// 	curr_local_env = le;
// }
