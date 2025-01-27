/****************************************************************/
/* This module implements the "inspect" directive, that is used */
/* to visit the graph representing a term.                      */
/* We plan to integrate the i/o with the YACC specification.    */
/* There is only one external function:                         */
/*  - inspect_driver(): this is the driver routine. It is       */
/*                      composed by a main cycle asking for the */
/*                      next direction to follow in the graph   */
/*                      (the port we must exit the curent form).*/
/* The following function is internal:                          */
/*  - inspect(): it prints informations about the next form     */
/*               encountered during the inspection of the graph */
/*               (its name, index, and the port we arrived to). */
/****************************************************************/

/****************************************************************/
/* Inclusion of header files.       				*/
/****************************************************************/

#include "bohm.h"

/****************************************************************/
/* Definitions of functions to be exported.			*/
/****************************************************************/

void Form::inspect_driver()
{
	// if (f == NULL)
	// {
	// 	printf("******************************************\n");
	// 	printf("* No terms inserted yet . . .            *\n");
	// 	printf("******************************************\n");
	// 	return;
	// }

	printf("**** inspection mode ****\n\n");
	printf("you are at ROOT node\n");
	printf("input next port to visit (0, 1, 2 or 3)\n");
	printf("input  q  to exit\n");

	Form *travel = this;
	int c = getchar();
	while (c != 'q')
	{
		while (c == ' ')
		{
			c = getchar();
		}
		if (c >= '0' && c <= '3')
		{
			travel = travel->inspect(c - '0');
		}
		c = getchar();
	}
	printf("**** end of inspection mode ****\n\n");
}

Form *Form::inspect(int p)
{
	bool ok = true;
	switch (p)
	{
	case 0:
		break;
	case 1:
		switch (this->kind)
		{
		case ERASE:
		case ROOT:
			ok = false;
			break;
		default:
			break;
		}
		break;
	case 2:
		switch (this->kind)
		{
		case ERASE:
		case ROOT:
		case INT:
		case T:
		case F:
		case NIL:
		case TRIANGLE:
		case LAMBDAUNB:
		case TESTNIL:
		case CAR:
		case CDR:
		case NOT:
		case EQ1:
		case NOTEQ1:
		case LEQ1:
		case MEQ1:
		case MORE1:
		case LESS1:
		case ADD1:
		case SUB1:
		case PROD1:
		case DIV1:
		case MOD1:
		case UNS_FAN1:
		case UNS_FAN2:
			ok = false;
			break;
		default:
			break;
		}
		break;
	case 3:
		switch (this->kind)
		{
		case IFELSE:
			break;
		default:
			ok = false;
			break;
		}
		break;
	default:
		ok = false;
		break;
	}

	if (!ok)
	{
		printf("there is nothing there\n");
		return this;
	}
	int nextport = this->nport[p];

	if (nextport < 0)
	{
		switch (nextport)
		{
		case T:
			printf("form = T\n");
			break;
		case F:
			printf("form = F\n");
			break;
		case INT:
			printf("form = INT value = %" PRIdPTR "\n", (intptr_t)this->nform[0]);
			break;
		case NIL:
			printf("form = NIL\n");
			break;
		}
		printf("at port = 0\n\n");
		return this;
	}

	Form *nextform = this->nform[p];
	switch (nextform->kind)
	{
	case FAN:
		printf("form = FAN\n");
		break;
	case TRIANGLE:
		printf("form = TRIANGLE\n");
		break;
	case ROOT:
		printf("form = ROOT\n");
		break;
	case APP:
		printf("form = APP\n");
		break;
	case LAMBDA:
		printf("form = LAMBDA\n");
		break;
	case IFELSE:
		printf("form = IFELSE\n");
		break;
	case AND:
		printf("form = AND\n");
		break;
	case OR:
		printf("form = OR\n");
		break;
	case NOT:
		printf("form = NOT\n");
		break;
	case LESS:
		printf("form = LESS\n");
		break;
	case LESS1:
		printf("form = LESS1 value = %" PRIdPTR "\n", (intptr_t)nextform->nform[2]);
		break;
	case EQ:
		printf("form = EQ\n");
		break;
	case EQ1:
		printf("form = EQ1 value = %" PRIdPTR "\n", (intptr_t)nextform->nform[2]);
		break;
	case NOTEQ:
		printf("form = NOTEQ\n");
		break;
	case NOTEQ1:
		printf("form = NOTEQ1 value = %" PRIdPTR "\n", (intptr_t)nextform->nform[2]);
		break;
	case MORE:
		printf("form = MORE\n");
		break;
	case MORE1:
		printf("form = MORE1 value = %" PRIdPTR "\n", (intptr_t)nextform->nform[2]);
		break;
	case LEQ:
		printf("form = LEQ\n");
		break;
	case LEQ1:
		printf("form = LEQ1 value = %" PRIdPTR "\n", (intptr_t)nextform->nform[2]);
		break;
	case MEQ:
		printf("form = MEQ\n");
		break;
	case MEQ1:
		printf("form = MEQ1 value = %" PRIdPTR "\n", (intptr_t)nextform->nform[2]);
		break;
	case ADD:
		printf("form = ADD\n");
		break;
	case ADD1:
		printf("form = ADD1 value = %" PRIdPTR "\n", (intptr_t)nextform->nform[2]);
		break;
	case SUB:
		printf("form = SUB\n");
		break;
	case SUB1:
		printf("form = SUB1 value = %" PRIdPTR "\n", (intptr_t)nextform->nform[2]);
		break;
	case PROD:
		printf("form = PROD\n");
		break;
	case PROD1:
		printf("form = PROD1 value = %" PRIdPTR "\n", (intptr_t)nextform->nform[2]);
		break;
	case DIV:
		printf("form = DIV\n");
		break;
	case DIV1:
		printf("form = DIV1 value = %" PRIdPTR "\n", (intptr_t)nextform->nform[2]);
		break;
	case MOD:
		printf("form = MOD\n");
		break;
	case MOD1:
		printf("form = MOD1 value = %" PRIdPTR "\n", (intptr_t)nextform->nform[2]);
		break;
	case CONS:
		printf("form = CONS\n");
		break;
	case CAR:
		printf("form = CAR\n");
		break;
	case CDR:
		printf("form = CDR\n");
		break;
	case TESTNIL:
		printf("form = TESTNIL\n");
		break;
	case LAMBDAUNB:
		printf("form = LAMBDAUNB\n");
		break;
	case UNS_FAN1:
		printf("form = UNS_FAN1\n");
		break;
	case UNS_FAN2:
		printf("form = UNS_FAN2\n");
		break;
	case CAR1:
		printf("form = CAR1\n");
		break;
	case CDR1:
		printf("form = CDR1\n");
		break;
	case TESTNIL1:
		printf("form = TESTNIL1\n");
		break;
	case CONS1:
		printf("form = CONS1\n");
		break;
	}
	printf("with index = %d\n", nextform->index);
	printf("at port = %d\n\n", nextport);
	return nextform;
}

/****************************************************************/
/* Definitions of functions strictly local to the module        */
/****************************************************************/
