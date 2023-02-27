/****************************************************************/
/* This modules carries out the storing of a graph on a textual */
/* file, to permit consultation.                                */
/* In particular, a list is stored of all the links existing    */
/* among the forms (specifying form types and ports), also      */
/* provided with an index of the forms and their associated     */
/* information. 					        */
/* The following function is external:                          */
/*  - save(): Saves a graph on a file.  			*/
/* The followinq functions are internal:                        */
/*  - present(): Checks whether a form has already been copied  */
/*               once.                                          */
/*  - stampa(): Saves on file a link				*/
/*  - save_aux(): Saves any graph part.				*/
/*  - put_form(): Prints form name.				*/
/*  - put_int(): Prints NIL, INT and BOOL forms names.		*/
/*  - num_port(): Returns a form's ports number.		*/
/*  - eindex(): Saves a file index row				*/
/****************************************************************/

/****************************************************************/
/* 1. Inclusion of header files.				*/
/****************************************************************/

#define ENTRY 17
#define NUM 13

#include "bohm.h"

/****************************************************************/
/* 2. Inclusion of declarations that are being imported.        */
/****************************************************************/

/****************************************************************/
/* 3. Declaration of names strictly local to the module.	*/
/****************************************************************/

FILE *save_file;
ELEM *head, *tail;
int max;

static void save_aux(
	FORM *root,
	int p);
static void put_int(
	FORM *f,
	int p);
static int num_port(int name);
static void eindex(ELEM *elem);

/****************************************************************/
/* 4. Definitions of functions to be exported.			*/
/****************************************************************/

/* The following function saves a graph on a file.		*/
void FORM::save(
	char *name,
	char *id)
{
	// if (this == NULL)
	// {
	// 	printf("******************************************\n");
	// 	printf("* No terms inserted yet . . .            *\n");
	// 	printf("******************************************\n");
	// 	return;
	// }
	save_file = fopen(name, "w");
	if (save_file == NULL)
	{
		exit(0);
	}
	head = tail = NULL;
	max = 1;
	this->stampa(0, this->present());
	if (this->nport[0] >= 0)
		save_aux(this->nform[0], this->nport[0]);
	ELEM *p = head;
	fprintf(save_file, "\n\n\nI N D E X :\n\n");
	while (p != NULL)
	{
		eindex(p);
		ELEM *dep = p;
		p = p->next;
		free(dep);
	}
	fclose(save_file);
	if (id == NULL)
	{
		printf("\nSaved last input term in the file %s.\n", name);
	}
	else
	{
		printf("\nSaved %s term in the file %s.\n", id, name);
	}
}

/****************************************************************/
/* 5. Definitions of functions strictly local to the module.	*/
/****************************************************************/

/* The following function checks whether a form has already 	*/
/* been copied once.						*/
int FORM::present()
{
	int risp = true;

	ELEM *p = head;
	while (p != NULL && risp)
	{
		if (p->node == this)
		{
			risp = false;
		}
		else
		{
			p = p->next;
		}
	}

	if (!risp)
	{
		return risp;
	}

	ELEM *alloc = (ELEM *)malloc_da(sizeof(ELEM));

	if (head == NULL)
	{
		head = tail = alloc;
	}
	else
	{
		tail->next = alloc;
		tail = tail->next;
	}
	tail->node = this;
	tail->next = NULL;
	tail->num = risp = max++;
	return risp;
}

/* The following function saves on file a link			*/
void FORM::stampa(
	int p,
	int card)
{
	const int p1 = this->nport[p];
	fprintf(save_file, "%4d ", card);
	this->put_form();
	if (p1 < 0)
	{
		fprintf(save_file, "%d -> 0 ", p);
		put_int(this->nform[p], p1);
	}
	else
	{
		fprintf(save_file, "%d -> %d ", p, p1);
		this->nform[p]->put_form();
	}
	fprintf(save_file, "\n");
}

/* The following function saves any graph part.			*/
static void save_aux(
	FORM *root,
	int p)
{
	int card = root->present();
	if (!card)
	{
		return;
	}
	int n = num_port(root->kind);

	for (int p1 = 0; p1 < n; p1++)
	{
		root->stampa(p1, card);
	}

	for (int p1 = 0; p1 < n; p1++)
	{
		if (root->nport[p1] >= 0)
		{
			save_aux(root->nform[p1], root->nport[p1]);
		}
	}
}

/* The following function prints form name.			*/
void FORM::put_form()
{
	fprintf(save_file, kind_desc(this->kind));
}

/* The following function prints NIL, INT and BOOL forms names.	*/
static void put_int(FORM *f, const int p)
{
	switch (p)
	{
	case T:
		fprintf(save_file, "True          ");
		break;
	case F:
		fprintf(save_file, "False         ");
		break;
	case INT:
		fprintf(save_file, "Int: %" PRIdPTR " ", (intptr_t)f);
		break;
	case NIL:
		fprintf(save_file, "Nil          ");
		break;
	}
}

/* The following function returns a form's ports number.	*/
static int num_port(int name)
{
	int risp;
	switch (name)
	{
	case ROOT:
		risp = 1;
		break;
	case TRIANGLE:
	case NOT:
	case LESS1:
	case EQ1:
	case NOTEQ1:
	case MORE1:
	case LEQ1:
	case MEQ1:
	case ADD1:
	case SUB1:
	case PROD1:
	case DIV1:
	case MOD1:
	case CAR:
	case CDR:
	case TESTNIL:
	case LAMBDAUNB:
	case UNS_FAN1:
	case UNS_FAN2:
		risp = 2;
		break;
	case FAN:
	case APP:
	case LAMBDA:
	case IFELSE:
	case AND:
	case OR:
	case LESS:
	case EQ:
	case NOTEQ:
	case MORE:
	case ADD:
	case LEQ:
	case MEQ:
	case SUB:
	case PROD:
	case DIV:
	case MOD:
	case CONS:
	case CAR1:
	case CDR1:
	case TESTNIL1:
	case CONS1:
		risp = 3;
		break;
	}
	return risp;
}

/* The following function saves a file index row		*/
static void eindex(ELEM *elem)
{
	fprintf(save_file, "%3d ", elem->num);
	elem->node->put_form();
	fprintf(save_file, "index: %2d", elem->node->index);
	switch (elem->node->kind)
	{
	case FAN:
	case CAR1:
	case CDR1:
	case TESTNIL1:
		fprintf(save_file, " nlevel[1]: %2d", elem->node->nlevel[1]);
		fprintf(save_file, " nlevel[2]: %2d", elem->node->nlevel[2]);
		break;

	case TRIANGLE:
	case UNS_FAN1:
	case UNS_FAN2:
		fprintf(save_file, " nlevel[1]: %2d", elem->node->nlevel[1]);
		break;

	case LESS1:
	case EQ1:
	case NOTEQ1:
	case MORE1:
	case LEQ1:
	case MEQ1:
	case ADD1:
	case SUB1:
	case PROD1:
	case DIV1:
	case MOD1:
		fprintf(save_file, " value %-d", elem->node->num_safe);
		break;
	}
	fprintf(save_file, "\n");
}
