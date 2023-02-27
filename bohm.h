#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <inttypes.h>
#include <assert.h>
#include <list>
#include <vector>

#include "form.h"
#include "struct.h"

extern bool error_detected;
extern bool loading_mode;
extern bool quit;
extern bool seegarb;
extern bool seenode;
extern bool seetime;
extern bool seenumber;
extern char *include_file;
extern FORM *del_head;
extern FORM *headfree;
extern FORM *lastinputterm;
extern FORM *current_pos;
extern int lines;
extern int option;
extern unsigned limit;
extern unsigned max_nodes;
extern unsigned num_nodes;

#ifdef __cplusplus
extern "C"
{
#endif

    int yylex();
    int yyerror(const char *msg);

#ifdef __cplusplus
}
#endif

char *malloc_da(unsigned size);
char *strdup_da(const char *s);

FORM *copy(
    FORM *root,
    int p,
    int offset);

VARLIST *makevarlist(
    STBUCKET *e,
    TERM *t);

VARLIST *mergevarlist(
    VARLIST *l1,
    VARLIST *l2);

void clean(void);

void bool_connect(
    FORM *form1,
    int portf1,
    int portf2);

void connect(
    FORM *form1,
    int portf1,
    FORM *form2,
    int portf2);

void connect1(
    FORM *form1,
    int portf1,
    FORM *form2,
    int portf2);

void int_connect(
    FORM *form1,
    int portf1,
    FORM *form2,
    int portf2);

void create_variable_binding(
    STBUCKET *st,
    FORM *rootform);

void destroy(void);
void info(char *choices);
void init_destroy(void);
void init_garbage(void);
void init_symbol_table(void);
// void ins_del(FORM *d);
// void inspect_driver(FORM *f);

void menu(int choice);
// void myfree(FORM *form);
void no_destroy(void);
void pop_local_env(void);
void push_local_env(void);
// void rdbk(FORM *form);
// void reduce_term(FORM *root);
void reset_garbage(void);
// void save(
//     char *name,
//     FORM *root,
//     char *id);
STBUCKET *search_bucket(const char *id);
void show_garb_stat(bool seetime);
void signal_error(const char *msg);
void user(void);
