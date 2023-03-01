#pragma once

#define ROOT 0
#define T -1
#define F -2
#define NIL -3
#define INT -4
#define ERASE 5
#define NOT 6
#define TRIANGLE 9
#define NOTEQ 10
#define NOTEQ1 11
#define ADD1 12
#define SUB1 13
#define PROD1 14
#define DIV1 15
#define MOD1 16
#define LESS1 17
#define EQ1 18
#define MORE1 19
#define LEQ1 20
#define MEQ1 21
#define CAR 22
#define CDR 23
#define TESTNIL 24
#define LAMBDAUNB 25
#define UNS_FAN1 27
#define UNS_FAN2 28
#define APP 29
#define LAMBDA 30
#define FAN 32
#define AND 33
#define OR 34
#define ADD 35
#define SUB 36
#define PROD 37
#define DIV 38
#define MOD 39
#define LESS 40
#define EQ 41
#define MORE 42
#define LEQ 43
#define MEQ 44
#define CONS 45
#define IFELSE 46
#define TESTNIL1 47
#define CDR1 48
#define CAR1 49
#define CONS1 50

inline int propagate_kind(int kind)
{
    switch (kind)
    {
    // case CAR1: return CAR;
    case CAR1:
        return CAR;
    case CDR1:
        return CDR;
    case TESTNIL1:
        return TESTNIL;
    default:
        assert(false);
    }
    // return CAR1;
}

inline int unpropagate_kind(int kind)
{
    switch (kind)
    {
    // case CAR1: return CAR;
    case CAR:
        return CAR1;
    case CDR:
        return CDR1;
    case TESTNIL:
        return TESTNIL1;
    default:
        assert(false);
    }
    // return CAR1;
}

inline const char *kind_desc(char kind)
{
    switch (kind)
    {
    case FAN:
        return "FAN       ";
    case TRIANGLE:
        return "TRIANGLE  ";
    case ROOT:
        return "ROOT      ";
    case APP:
        return "APP       ";
    case LAMBDA:
        return "LAMBDA    ";
    case IFELSE:
        return "IFELSE    ";
    case AND:
        return "AND       ";
    case OR:
        return "OR        ";
    case NOT:
        return "NOT       ";
    case LESS:
        return "LESS      ";
    case LESS1:
        return "LESS1     ";
    case EQ:
        return "EQ        ";
    case EQ1:
        return "EQ1       ";
    case NOTEQ:
        return "NOTEQ     ";
    case NOTEQ1:
        return "NOTEQ1    ";
    case MORE:
        return "MORE      ";
    case MORE1:
        return "MORE1     ";
    case LEQ:
        return "LEQ       ";
    case LEQ1:
        return "LEQ1      ";
    case MEQ:
        return "MEQ       ";
    case MEQ1:
        return "MEQ1      ";
    case ADD:
        return "ADD       ";
    case ADD1:
        return "ADD1      ";
    case SUB:
        return "SUB       ";
    case SUB1:
        return "SUB1      ";
    case PROD:
        return "PROD      ";
    case PROD1:
        return "PROD1     ";
    case DIV:
        return "DIV       ";
    case DIV1:
        return "DIV1      ";
    case MOD:
        return "MOD       ";
    case MOD1:
        return "MOD1      ";
    case CONS:
        return "CONS      ";
    case CAR:
        return "CAR       ";
    case CDR:
        return "CDR       ";
    case TESTNIL:
        return "TESTNIL   ";
    case LAMBDAUNB:
        return "LAMBDAUNB ";
    case UNS_FAN1:
        return "UNS_FAN1  ";
    case UNS_FAN2:
        return "UNS_FAN2  ";
    case CAR1:
        return "CAR1      ";
    case CDR1:
        return "CDR1      ";
    case TESTNIL1:
        return "TESTNIL1  ";
    case CONS1:
        return "CONS1     ";
    }
    assert(false);
}