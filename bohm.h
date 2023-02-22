#pragma once

#include "form.h"
#include "struct.h"

#include <stdbool.h>

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

extern char *malloc_da(unsigned size);
extern char *strdup_da(char *s);
extern FORM *closeterm(
    int level,
    TERM *t);

extern FORM *copy(
    FORM *root,
    int p,
    int offset);

extern FORM *inspect(int p, FORM *f);

extern TERM *buildandterm(int level,
                          TERM *arg1,
                          TERM *arg2);

extern TERM *buildappterm(
    int level,
    TERM *fun,
    TERM *arg);

extern TERM *buildcarterm(
    int level,
    TERM *arg);

extern TERM *buildcdrterm(
    int level,
    TERM *arg);

extern TERM *buildfalseterm(int level);

TERM *buildifelseterm(
    int level,
    TERM *arg1,
    TERM *arg2,
    TERM *arg3);

TERM *buildintterm(
    int level, long int value);

extern TERM *buildletinterm(
    int level,
    STBUCKET *id,
    TERM *arg1,
    TERM *arg2);

extern TERM *buildlist(
    int level,
    TERM *arg1,
    TERM *arg2);

extern TERM *buildlist1(
    int level,
    TERM *arg1,
    TERM *arg2);

extern TERM *buildmatterm(
    int level,
    TERM *arg1,
    TERM *arg2,
    int op);

extern TERM *buildminusterm(
    int level,
    TERM *arg1);

extern TERM *build_mu_term(
    int level,
    STBUCKET *id,
    TERM *bod);

extern TERM *buildnillist(int level);

extern TERM *buildnotterm(
    int level,
    TERM *arg);

extern TERM *buildorterm(
    int level,
    TERM *arg1,
    TERM *arg2);

extern TERM *buildplambdaterm(
    int level,
    PATTERN *pattern,
    TERM *body);

extern TERM *buildrelopterm(
    int level,
    TERM *arg1,
    TERM *arg2,
    int relop);

extern TERM *buildtestnil(
    int level,
    TERM *arg);

extern TERM *buildtrueterm(int level);

extern TERM *buildvarterm(
    int level, STBUCKET *id);

extern TERM *buildvoidterm(int level);

extern VARLIST *makevarlist(
    STBUCKET *e,
    TERM *t);

extern VARLIST *mergevarlist(
    VARLIST *l1, VARLIST *l2);

FORM *allocate_form(
    int name,
    int index);

extern void clean(void);

extern void bool_connect(
    FORM *form1,
    int portf1,
    int portf2);

extern void connect(
    FORM *form1,
    int portf1,
    FORM *form2,
    int portf2);

extern void connect1(
    FORM *form1,
    int portf1,
    FORM *form2,
    int portf2);

extern void int_connect(
    FORM *form1,
    int portf1,
    FORM *form2,
    int portf2);

extern void create_variable_binding(
    STBUCKET *st, FORM *rootform);
extern void destroy(void);
extern void free_pattern(PATTERN *p);
extern void info();
extern void init_destroy(void);
extern void init_garbage(void);
extern void init_symbol_table(void);
extern void ins_del(FORM *d);
extern void inspect_driver(FORM *f);

extern void menu();
extern void myfree(FORM *form);
extern void no_destroy(void);
extern void pop_local_env(void);
extern void push_local_env(void);
extern void rdbk();
extern void reduce_term(FORM *root);
extern void reset_garbage(void);
extern void save();
extern STBUCKET *search_bucket(char *id);
void show_garb_stat(bool seetime);
extern void signal_error(const char *msg);
extern void user(void);
