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

VARLIST *makevarlist(
    STBUCKET *e,
    TERM *t);

VARLIST *mergevarlist(
    VARLIST *l1,
    VARLIST *l2);

void clean(void);

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

struct Destroyer final
{
private:
    FORM *headfree;
    FORM *headfull;
    unsigned start_nodes;

public:
    Destroyer();
    void destroy();
    void no_destroy();

    FORM *alloc();
    void release(FORM *form);
};

#define DICTSIZE 211

struct SymbolTable final
{

    LOCALENVENTRY *curr_local_env;
    SymbolTable();

    void pop_local_env();
    void push_local_env();

    STBUCKET *search_bucket(const char *id);

private:
    STBUCKET *dictionary[DICTSIZE];
    int curr_nesting_depth;

    void reset();
    STBUCKET *allocate_bucket(const char *id);
    void move_bucket(
        STBUCKET *st,
        int dict_index);
    void allocate_local_env_entry();
};

inline SymbolTable st = SymbolTable();

// void destroy(void);
void info(char *choices);
inline Destroyer destroyer = Destroyer();
// void init_destroy(void);
void init_garbage(void);
// void init_symbol_table(void);

void menu(int choice);
// void pop_local_env(void);
// void push_local_env(void);
void reset_garbage(void);
// STBUCKET *search_bucket(const char *id);
void show_garb_stat(bool seetime);
void signal_error(const char *msg);
void user(void);
