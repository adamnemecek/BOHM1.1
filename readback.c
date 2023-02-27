/****************************************************************/
/* This module implements the readback procedure as defined     */
/* is Asperti Laneve: Interaction Systems II.                   */
/* The only external function is                                */
/* - rdbk():     it provides the readback in standard syntax    */
/*               of the graphical term whose root is passed     */
/*               as input parameter.                            */
/****************************************************************/

/****************************************************************/
/* Inclusion of header files.	  			        */
/****************************************************************/

#include "bohm.h"

#define PRINT_MAX 100

static int left_to_print;
/* maximum number of characters yet to print */

void rdbk_1(
    FORM *form,
    int port);

void rdbk_list(
    FORM *form,
    int port);

/* the following function prints on the standard output the */
/* standard syntactical representation of the graphical term */
/* whose root is passed in input */
void FORM::rdbk()
{
  left_to_print = PRINT_MAX;
  printf("  ");
  rdbk_1(this, 0);
  printf("\n");
}

void rdbk_1(
    FORM *form,
    int port)
{
  if (left_to_print > 0)
    if (form->nport[port] < 0)
    {
      switch (form->nport[port])
      {
      case INT:
        left_to_print -= printf("%" PRIdPTR, (intptr_t)form->nform[0]);
        break;
      case T:
        left_to_print -= printf("TRUE");
        break;
      case F:
        left_to_print -= printf("FALSE");
        break;
      case NIL:
        left_to_print -= printf("[]");
        break;
      default:
        left_to_print -= printf("...");
      }
    }
    else
      switch (form->nform[port]->kind)
      {
      case LAMBDA:
      case LAMBDAUNB:
        if (form->nport[port] == 0)
          left_to_print -= printf("#<function>");
        else
          left_to_print -= printf("...");
        break;
      case CONS:
        if (form->nport[port] == 0)
        {
          left_to_print -= printf("[");
          rdbk_1(form->nform[port], 1);
          form->nform[port]->rdbk_list(2);
        }
        else
        {
          left_to_print -= printf("...");
        }
        break;
      case FAN:
        if (form->nport[port] != 0)
          rdbk_1(form->nform[port], 0);
        else
        {
          left_to_print -= printf("...");
        }
        break;
      case TRIANGLE:
        rdbk_1(form->nform[port], !form->nport[port]);
        break;
      default:
        left_to_print -= printf("...");
      }
  else
    left_to_print -= printf("...");
}

void FORM::rdbk_list(int port)
{
  if ((int)this->nport[port] == NIL)
    left_to_print -= printf("]");
  else if (left_to_print <= 0)
    left_to_print -= printf("...]");
  else if (this->nport[port] < 0)
  {
    left_to_print -= printf("|");
    rdbk_1(this, port);
    left_to_print -= printf("]");
  }
  else if (this->nform[port]->kind == TRIANGLE ||
           (this->nform[port]->kind == FAN && this->nport[port] != 0))
    this->nform[port]->rdbk_list(!this->nport[port]);
  else if (this->nform[port]->kind != CONS || this->nport[port] != 0)
  {
    left_to_print -= printf("|");
    rdbk_1(this, port);
    left_to_print -= printf("]");
  }
  else
  {
    left_to_print -= printf(",");
    rdbk_1(this->nform[port], 1);
    this->nform[port]->rdbk_list(2);
  }
}
