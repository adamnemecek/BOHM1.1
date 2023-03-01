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
// extern FORM *del_head;
// extern FORM *headfree;
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

// void clean(void);

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
public:
    Destroyer();
    void destroy();
    void no_destroy();

    FORM *alloc();
    void release(FORM *form);

private:
    FORM *headfree;
    FORM *headfull;
    unsigned start_nodes;
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

struct Garbage final
{
public:
    FORM *del_head;

    Garbage();

    void del(FORM *form);

    void clean();

    void reset(void);
    void show_stats(bool seetime);

private:
    long unsigned er_count;
    long unsigned cl_count;
    clock_t usr_garb_time;
    clock_t sys_garb_time;
    void garbage(FORM *form);
};

inline SymbolTable st = SymbolTable();
inline Destroyer destroyer = Destroyer();
inline Garbage gc = Garbage();

void info(char *choices);
void menu(int choice);
void signal_error(const char *msg);
void user(void);
