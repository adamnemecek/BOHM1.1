/****************************************************************/
/* This module supplies routines for dynamic allocation.	*/
/* It exports the following functions:                          */
/* - malloc_da(): it allocates a given amount of bytes; if it	*/
/*		  is unable to allocate, it signals this fact;	*/
/* - strdup_da(): it allocates a given string; if it is unable	*/
/*		  to allocate, it signals this fact.		*/
/****************************************************************/

/****************************************************************/
/* 1. Inclusion of header files.				*/
/****************************************************************/

#include "bohm.h"

/****************************************************************/
/* 2. Inclusion of declarations that are being imported.        */
/****************************************************************/

/****************************************************************/
/* 3. Definitions of variables to be exported.			*/
/****************************************************************/

/****************************************************************/
/* 4. Definitions strictly local to the module.                 */
/****************************************************************/

/* constants concerning crash handling */
#define COMPILERCRASH 2
#define NOTENOUGHMEMORY 0

const char *crash_msgs[] = {
	"not enough memory",
	"unable to open this file"};

/* The following function signals errors causing abort. */
static void signal_crash(int crash_type) /* crash type */
{
	/* crash messages */

	fprintf(stderr, "%s\n", crash_msgs[crash_type]);
	exit(COMPILERCRASH);
}

/****************************************************************/
/* 5. Definitions of functions to be exported.			*/
/****************************************************************/

/* The following function implements a control interface for the */
/* library function malloc(). */
char *malloc_da(unsigned size)
/* size of the object to be */
/* allocated */
{
	char *p = (char *)malloc(size);
	if (!p)
		signal_crash(NOTENOUGHMEMORY);
	return p;
}

/* The following function implements a control interface for the */
/* library function strdup(). */
char *strdup_da(const char *s)
{
	char *p = strdup(s);
	if (!p)
		signal_crash(NOTENOUGHMEMORY);
	return p;
}
