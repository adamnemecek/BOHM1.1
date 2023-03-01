/******************************************************************/
/* This module implements the destroyer operator, that is to be	  */
/* applied  at the end of a term. 				  */
/* If the last term is a global defination the graph is stored,   */
/* otherwise each node is freed. In order to do this, each node   */
/* is kept in a bi-linked list that is provided with two access   */
/* points: 							  */
/* - "headfull": pointer to the global definition's last element  */
/* - "headfree": pointer to the first idle node			  */
/* Only and all the last term's nodes are present between	  */
/* headfull and headfree. If the last term is a definition, 	  */
/* headfull has to be moved to the node preceding headfree, if 	  */
/* the last term is to be deleted, headfree is moved to the node  */
/* following headfull. 						  */
/* The following functions are external:                          */
/* - init_destroy(): Initialises the destroyer by allocating two  */
/*                   nodes (headfree and headfull) and linking    */
/*		     them together.                               */
/* - destroy(): Eliminates the preceding term, moving back        */
/*		headfree to the node following headfull, and      */
/*		doing so makes all the nodes available for	  */
/*		any furure usage				  */
/* - no_destroy(): Makes a graph associated to a global definition*/
/*                 permanent, by moving headfull to the node      */
/*                 preceding headfree.                            */
/******************************************************************/

/******************************************************************/
/* 1. Inclusion of header files                                   */
/******************************************************************/

#include "bohm.h"

/****************************************************************/
/* 2. Definitions of variables to be exported.			*/
/****************************************************************/

/* 3. Declaration of names strictly local to the module.	*/
/****************************************************************/

/* constants concerning allocate form */
#define FORM_NUM 1000
/* The following function initialises the destroyer by 	*/
/* allocating two nodes (headfree and headfull) and 	*/
/* linking them together.                               */
Destroyer::Destroyer()
{
  headfull = (Form *)malloc_da(sizeof(Form));
  headfree = (Form *)malloc_da(sizeof(Form));
  headfull->next = headfree;
  headfull->prev = NULL;
  headfree->next = NULL;
  headfree->prev = headfull;
  start_nodes = 0;

  Form *dep = (Form *)malloc_da(sizeof(Form) * FORM_NUM);
  // auto *dep = new Form[FORM_NUM];
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
/* The following function eliminates the preceding 	*/
/* term, moving back headfree to the node following 	*/
/* headfull, and doing so makes all the nodes available */
/* for any furure usage					*/
void Destroyer::destroy()
{
  if (headfree != headfull->next)
  {
    num_nodes = start_nodes;
    headfree = headfull->next;
  }
  start_nodes = num_nodes;
  gc.del_head->nform[1] = NULL;
}

/* The following function makes a graph associated to a	*/
/* global definition permanent, by moving headfull to 	*/
/* the node preceding headfree.				*/
void Destroyer::no_destroy(void)
{
  headfull = headfree->prev;
}

Form *Destroyer::alloc()
{
  Form *ret = headfree;
  headfree = headfree->next;
  return ret;
}

void Destroyer::release(Form *form)
{
  form->next = headfree->next;
  form->prev = headfree;
  if (headfree->next != NULL)
  {
    headfree->next->prev = form;
  }
  headfree->next = form;
}

/****************************************************************/
/* 4. Declaration of functions strictly local to the module.	*/
/****************************************************************/

/****************************************************************/
/* 5. Definitions of functions to be exported.			*/
/****************************************************************/
