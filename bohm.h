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
// extern Form *del_head;
// extern Form *headfree;
extern Form *lastinputterm;
extern Form *current_pos;
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

VarList *makevarlist(
    StBucket *e,
    Term *t);

VarList *mergevarlist(
    VarList *l1,
    VarList *l2);

// void clean(void);

void connect(
    Form *form1,
    int portf1,
    Form *form2,
    int portf2);

void connect1(
    Form *form1,
    int portf1,
    Form *form2,
    int portf2);

void int_connect(
    Form *form1,
    int portf1,
    Form *form2,
    int portf2);

struct Destroyer final
{
public:
    Destroyer();
    void destroy();
    void no_destroy();

    Form *alloc();
    void release(Form *form);

private:
    Form *headfree;
    Form *headfull;
    unsigned start_nodes;
};

#define DICTSIZE 211

struct SymbolTable final
{
    LocalEnvEntry *curr_local_env;
    SymbolTable();

    void pop_local_env();
    void push_local_env();

    StBucket *search_bucket(const char *id);

private:
    StBucket *dictionary[DICTSIZE];
    int curr_nesting_depth;

    void reset();
    StBucket *allocate_bucket(const char *id);
    void move_bucket(
        StBucket *st,
        int dict_index);
    void allocate_local_env_entry();
};

struct Garbage final
{
public:
    Form *del_head;

    Garbage();

    void del(Form *form);

    void clean();

    void reset(void);
    void show_stats(bool seetime);

private:
    long unsigned er_count;
    long unsigned cl_count;
    clock_t usr_garb_time;
    clock_t sys_garb_time;
    void garbage(Form *form);
};

inline SymbolTable st = SymbolTable();
inline Destroyer destroyer = Destroyer();
inline Garbage gc = Garbage();

void info(char *choices);
void menu(int choice);
void signal_error(const char *msg);
void user(void);
