//**************************************************************
// This module implements the interaction-net rewriting rules
// for reducing the graph representing a lambda term.
// Reduction is "weak". It looks for the leftmost outermost
// redex, following the main spine of the term. In no redex is
// found, termination stops there. This is essentially
// equivalent to never reduce inside a lambda: the only
// difference is that, since the implementation is optimal,
// family reduction is always performed (so, "some" redexes
// inside a lambda can be reduced).
// During the search for the leftmost outermost redex, the main
// spine is saved on an auxiliary stack (this is similar to the
// G-machine).
// The two variables "redexes" and "counter" are respectively
// used to hold the number of app-lambda interactions and the
// total number of interactions during the reduction.
// The following function are external.
//  - reduce_term: reduces a term to its weak head (family)
//                 normal form.
// The following functions are strictly local to the module:
//  - reduce_term(): it reduces the input term
//  - reduce_redex(): it reduces the redex whose interacting
//                    forms are passed as a parameter and the
//		      second form is a INT, NIL, True or False;
//  - lo_redex(): it looks for the leftmost outemost redex,
//                saving on the m_stack pointers to the form
//                along the main spine of the term.
//**************************************************************

//**************************************************************
// 1. Inclusion of header files.
//**************************************************************

#include "bohm.h"

//**************************************************************
// 2. Inclusion of declarations that are being imported.
//**************************************************************

//**************************************************************
// 3. Definitions strictly local to the module.
//**************************************************************

static int unsafe;
static int optim;
static int fan_int;
static int redexes;
static int max_index;
static int eq = 0;
static int type_error;
static void reduce_redex(
	Form *f1,
	Form *f2);
// static void reduce_form(Form *f1);
// static Form *lo_redex(Form *f);
static int auxnext;

#ifndef STACK_SIZE
#define STACK_SIZE 10000000
#endif

struct Stack
{
private:
	Form *data[STACK_SIZE];
	int count;

public:
	Stack()
	{
		count = 0;
		*data = {0};
	}

	void push(Form *f)
	{
		data[count] = f;

		++count;
		if (count >= STACK_SIZE)
		{
			printf("Stack Overflow . . .\n");
			getchar();
			getchar();
		}
	}

	Form *pop()
	{
		assert(count > 0);
		--count;
		Form *res = data[count];
		data[count] = NULL;
		return res;
	}
};

Stack stack = Stack();

// static Form *auxstack[STACK_SIZE];

// static Form *pop()
// {
// 	Form *res;

// 	assert(auxnext > 0);
// 	--auxnext;
// 	res = auxstack[auxnext];
// 	auxstack[auxnext] = NULL;
// 	return res;
// }

// static void push(Form *f)
// {
// 	auxstack[auxnext] = f;

// 	++auxnext;
// 	if (auxnext >= STACK_SIZE)
// 	{
// 		printf("Stack Overflow . . .\n");
// 		getchar();
// 		getchar();
// 	}
// }

// static void init_stack()
// {
// 	auxnext = 0;
// }

//**************************************************************
// 4. Definitions of functions to be exported.
//**************************************************************

int fl = 0;
int counter;

//  The following function reduces a term to its weak
//  head (family) normal form.
void Form::reduce_term()
{
	struct tms time;

	unsafe = 0;
	optim = 0;
	fan_int = 0;
	counter = 0;
	eq = 0;
	redexes = 0;
	type_error = false;
	max_index = 0;
	if (seenode)
	{
		printf("\n*****************************************************\n");
		printf("Initial number of nodes %u\n", num_nodes);
		printf("*****************************************************\n");
	}
	times(&time);
	clock_t usr_time = time.tms_utime;
	clock_t sys_time = time.tms_stime;
	Form *f1 = this->lo_redex();
	// reset_garbage();
	gc.reset();
	while (f1 != this && !type_error)
	{
		if (f1->nport[0] == 0)
		{
			Form *f2 = f1->nform[0];
			if (f1->index <= f2->index)
			{
				reduce_redex(f1, f2);
				if (f2->index > max_index)
				{
					max_index = f2->index;
				}
			}
			else
			{
				reduce_redex(f2, f1);
				if (f1->index > max_index)
				{
					max_index = f1->index;
				}
			}
		}
		else
		{
			f1->reduce_form();
		}
		counter = counter + 1;
		f1 = stack.pop()->lo_redex();
	}
	if (type_error)
	{
		printf("--->   bad definition, sorry . . .\n");
		int ans = getchar();
		if (ans == 'i')
		{
			this->inspect_driver();
		}
		return;
	}

	times(&time);
	usr_time = time.tms_utime - usr_time;
	sys_time = time.tms_stime - sys_time;
	this->rdbk();
	if (seetime)
	{
		printf("*****************************************************\n");
		printf("Reduction done in %.2f:user    ", ((double)usr_time / 60));
		printf("%.2f:system seconds\n", ((double)sys_time / 60));
		printf("*****************************************************\n");
	}
	if (seenumber)
	{
		printf("*****************************************************\n");
		printf("Total number of interactions: %u\n", counter);
		printf("Total number of APP-LAMBDA redexes: %u\n", redexes);
		printf("Proper Interactions:%d\n", eq);
		printf("FAN-Interactions:%d\n", fan_int);
		printf("Optimization-Rules:%d\n", optim);
		printf("Unsafe-Rules:%d\n", unsafe);
		printf("Max index:%d\n", max_index);
		printf("*****************************************************\n");
	}
	if (option != 3 && seegarb)
	{
		gc.show_stats(seetime);
	}
	if (seenode)
	{
		printf("Max. number of nodes seen up to this time %u\n", max_nodes);
		printf("Final number of nodes %u\n", num_nodes);
		printf("*****************************************************\n");
	}
}

//**************************************************************
// 5. Definitions of functions strictly local to the module.
//**************************************************************

//  The following function reduces the redex whose
//  interacting forms are passed as a parameter and the
//  second form is not a INT, NIL, True or False;
static void reduce_redex(
	Form *f1,
	Form *f2)
{
	Form *new1;
	Form *new2;

	if (option == 2 && gc.del_head != NULL && num_nodes > limit)
	{
		gc.clean();
	}

	if (f1->index == f2->index)
	{
		if (f1->kind == f2->kind)
		{
			switch (f1->kind)
			{
			case FAN:
				if (f1->nlevel[1] != f2->nlevel[1] ||
					f1->nlevel[2] != f2->nlevel[2])
					printf("warning\n");
				connect1(f1->nform[1],
						 f1->nport[1],
						 f2->nform[1],
						 f2->nport[1]);
				connect1(f1->nform[2],
						 f1->nport[2],
						 f2->nform[2],
						 f2->nport[2]);
				f1->release();
				f2->release();
				fan_int++;
				break;

			case TRIANGLE:
				connect1(f1->nform[1],
						 f1->nport[1],
						 f2->nform[1],
						 f2->nport[1]);
				f1->release();
				f2->release();
				break;

			case UNS_FAN1:
			case UNS_FAN2:
				unsafe++;
				connect1(f1->nform[1],
						 f1->nport[1],
						 f2->nform[1],
						 f2->nport[1]);
				f1->release();
				f2->release();
				fan_int++;
				break;
			}
		}
		else
		{

			switch (f1->kind)
			{
			case APP:
				redexes = redexes + 1;

				switch (f2->kind)
				{
				case LAMBDA:
					eq++;
					connect1(f1->nform[1],
							 f1->nport[1],
							 f2->nform[1],
							 f2->nport[1]);
					connect1(f2->nform[2],
							 f2->nport[2],
							 f1->nform[2],
							 f1->nport[2]);
					f1->release();
					f2->release();
					break;
				case LAMBDAUNB:
					eq++;
					connect1(f1->nform[1],
							 f1->nport[1],
							 f2->nform[1],
							 f2->nport[1]);
					f1->kind = ERASE;
					connect1(f1, 0, f1->nform[2], f1->nport[2]);
					f1->del();
					if (option == 1)
					{
						gc.clean();
					}
					f2->release();
					break;
				default:
					printf("--->   type error1\n");
					type_error = true;
					break;
				}
				break;

			case CAR:
				switch (f2->kind)
				{
				case CONS:
					eq++;
					connect1(f1->nform[1],
							 f1->nport[1],
							 f2->nform[1],
							 f2->nport[1]);
					f1->kind = ERASE;
					connect1(f1, 0, f2->nform[2], f2->nport[2]);
					f1->del();
					if (option == 1)
					{
						gc.clean();
					}
					f2->release();
					break;
				default:
					printf("--->   type error28 %d\n", f2->kind);
					type_error = true;
					break;
				}
				break;

			case CAR1:
				switch (f2->kind)
				{
				case CONS:
					eq++;
					f1->index -= 1;
					f1->kind = FAN;
					connect(f1->nform[2], f1->nport[2], f2, 0);
					f1->connect1(0, f2->port(1));
					connect(f1, 2, f2, 1);
					if (f1->nlevel[2] != 0)
					{
						new1 = new Form(TRIANGLE, f1->index);
						new1->nlevel[1] = f1->nlevel[2];
						new1->connect1(0, f2->port(2));
						connect(new1, 1, f2, 2);
						f2->index += f1->nlevel[2];
					}
					break;
				default:
					printf("--->   type error28 bis %d\n", f2->kind);
					type_error = true;
					break;
				}
				break;

			case CDR:
				switch (f2->kind)
				{
				case CONS:
					eq++;
					connect1(f1->nform[1],
							 f1->nport[1],
							 f2->nform[2],
							 f2->nport[2]);
					f1->kind = ERASE;
					f1->connect1(0, f2->port(1));
					f1->del();
					if (option == 1)
					{
						gc.clean();
					}
					f2->release();
					break;
				default:
					printf("--->   type error\n");
					type_error = true;
					break;
				}
				break;

			case CDR1:
				switch (f2->kind)
				{
				case CONS:
					eq++;
					connect(f1->nform[2], f1->nport[2], f2, 0);
					f1->connect1(0, f2->port(2));
					connect(f1, 2, f2, 2);
					f1->kind = FAN;
					f1->index -= 1;
					if (f1->nlevel[2] != 0)
					{
						new1 = new Form(TRIANGLE, f1->index);
						new1->nlevel[1] = f1->nlevel[2];
						new1->connect1(0, f2->port(1));

						connect(new1, 1, f2, 1);
						f2->index += f1->nlevel[2];
					}
					break;
				default:
					printf("--->   type error\n");
					type_error = true;
					break;
				}
				break;

			case TESTNIL:
				switch (f2->kind)
				{
				case CONS:
					eq++;
					f1->bool_connect(1, F);
					f1->kind = ERASE;
					f1->del();
					connect(f1, 0, f2, 0);
					if (option == 1)
					{
						gc.clean();
					}
					break;
				default:
					printf("--->   type error\n");
					type_error = true;
					break;
				}
				break;

			case TESTNIL1:
				switch (f2->kind)
				{
				case CONS:
					eq++;
					connect(f1->nform[2], f1->nport[2], f2, 0);
					f1->bool_connect(1, F);
					if (f1->nlevel[2] != 0)
					{
						new1 = new Form(TRIANGLE, f1->index - 1);
						new1->nlevel[1] = f1->nlevel[2];
						new1->connect1(0, f2->port(1));
						connect(new1, 1, f2, 1);
						f1->kind = TRIANGLE;
						f1->index--;
						f1->nlevel[1] = f1->nlevel[2];
						f1->connect1(0, f2->port(2));
						connect(f1, 1, f2, 2);
						f2->index += f1->nlevel[2];
					}
					else
					{
						f1->release();
					}
					break;
				default:
					printf("--->   type error\n");
					type_error = true;
					break;
				}
				break;

			case CONS1:
				switch (f2->kind)
				{
				case CONS:
					if (is_connectable(f1->nport[1]))
					{
						connect1(f1->nform[1],
								 f1->nport[1],
								 f2->nform[1],
								 f2->nport[1]);
					}
					else
					{
						new1 = new Form(ERASE, 0);
						f1->del();
						new1->connect1(0, f2->port(1));
					}
					if (is_connectable(f1->nport[2]))
					{
						connect1(f1->nform[2],
								 f1->nport[2],
								 f2->nform[2],
								 f2->nport[2]);
						f2->release();
					}
					else
					{
						f2->kind = ERASE;
						f2->del();
						connect(f2, 0, f2->nform[1], f2->nport[1]);
					}
					f1->release();
					eq++;
					break;
				default:
					printf("--->   type error\n");
					type_error = true;
					break;
				}
				break;

			case UNS_FAN1:
				unsafe++;
				fan_int++;
				switch (f2->kind)
				{
				case FAN:
					connect1(f1->nform[1], f1->nport[1],
							 f2->nform[2], f2->nport[2]);
					f1->kind = ERASE;
					f1->connect1(0, f2->port(1));
					f1->del();
					if (option == 1)
					{
						gc.clean();
					}
					f2->release();
					break;

				case UNS_FAN2:
					f1->kind = ERASE;
					f2->kind = ERASE;
					f1->connect1(0, f1->port(1));
					f2->connect1(0, f2->port(1));
					f1->del();
					f2->del();
					if (option == 1)
						;
					gc.clean();
					break;
				default:
					printf("deadlock1\n");
					exit(1);
				}
				break;

			case UNS_FAN2:
				unsafe++;
				fan_int++;
				switch (f2->kind)
				{
				case FAN:
					connect1(f1->nform[1], f1->nport[1],
							 f2->nform[1], f2->nport[1]);
					f1->kind = ERASE;
					f1->connect1(0, f2->port(2));
					f1->del();
					if (option == 1)
					{
						gc.clean();
					}
					f2->release();
					break;

				case UNS_FAN1:
					f1->kind = ERASE;
					f2->kind = ERASE;
					f1->connect1(0, f1->port(1));
					f2->connect1(0, f2->port(1));
					new1->del();
					new2->del();
					if (option == 1)
						;
					gc.clean();
					break;
				default:
					printf("deadlock\n");
					exit(1);
				}
				break;

			case FAN:
				fan_int++;
				switch (f2->kind)
				{
				case UNS_FAN1:
					unsafe++;
					connect1(f1->nform[2], f1->nport[2],
							 f2->nform[1], f2->nport[1]);
					f1->kind = ERASE;
					f1->connect1(0, f1->port(1));
					f1->del();
					if (option == 1)
					{
						gc.clean();
					}
					f2->release();
					break;

				case UNS_FAN2:
					unsafe++;
					connect1(f1->nform[1], f1->nport[1],
							 f2->nform[1], f2->nport[1]);
					f1->kind = ERASE;
					f1->connect1(0, f1->port(2));
					f1->del();
					if (option == 1)
					{
						gc.clean();
					}
					f2->release();
					break;

				default:
					printf("deadlock\n");
					exit(1);
				}
				break;

			default:
				printf("deadlock\n");
				exit(1);
			}
		}
	}
	else
	{ // f1.index < f2.index
		switch (f1->kind)
		{
		case FAN:
			if (f2->kind != TRIANGLE)
			{
				fan_int++;
			}
			if (f2->kind == LAMBDA)
			{
				f1->num_safe = false;
			}
			switch (f2->kind)
			{
			case CONS:
			case EQ:
			case NOTEQ:
			case LEQ:
			case MEQ:
			case MORE:
			case LESS:
			case ADD:
			case SUB:
			case PROD:
			case DIV:
			case MOD:
			case AND:
			case OR:
			case IFELSE:
			case APP:
			case LAMBDA:
				new1 = new Form(f2->kind, f2->index + f1->nlevel[2]);
				f2->index += f1->nlevel[1];
				new2 = new Form(FAN, f1->index);
				new2->num_safe = f1->num_safe;
				new2->nlevel[1] = f1->nlevel[1];
				new2->nlevel[2] = f1->nlevel[2];

				f2->connect1(0, f1->port(1));
				new1->connect1(0, f1->port(2));
				new2->connect1(0, f2->port(1));
				f1->connect1(0, f2->port(2));
				connect(f1, 1, f2, 2);
				connect(f1, 2, new1, 2);
				connect(f2, 1, new2, 1);
				connect(new1, 1, new2, 2);
				break;

			case FAN:
			case TESTNIL1:
			case CAR1:
			case CDR1:
			case CONS1:
				new1 = new Form(f2->kind, f2->index + f1->nlevel[2]);
				f2->index += f1->nlevel[1];
				new1->num_safe = f2->num_safe;
				new1->nlevel[1] = f2->nlevel[1];
				new1->nlevel[2] = f2->nlevel[2];
				new2 = new Form(FAN, f1->index);
				new2->num_safe = f1->num_safe;
				new2->nlevel[1] = f1->nlevel[1];
				new2->nlevel[2] = f1->nlevel[2];

				f2->connect1(0, f1->port(1));
				new1->connect1(0, f1->port(2));
				new2->connect1(0, f2->port(1));
				f1->connect1(0, f2->port(2));

				connect(f1, 1, f2, 2);
				connect(f1, 2, new1, 2);
				connect(f2, 1, new2, 1);
				connect(new1, 1, new2, 2);
				break;

			case UNS_FAN1:
			case UNS_FAN2:
				unsafe++;
			case TRIANGLE:
				new1 = new Form(f2->kind, f2->index + f1->nlevel[2]);
				new1->num_safe = f2->num_safe;
				new1->nlevel[1] = f2->nlevel[1];
				f2->index += f1->nlevel[1];

				f2->connect1(0, f1->port(1));
				new1->connect1(0, f1->port(2));
				f1->connect1(0, f2->port(1));

				connect(f1, 1, f2, 1);
				connect(f1, 2, new1, 1);
				break;

			case TESTNIL:
			case CAR:
			case CDR:
			case NOT:
			case LAMBDAUNB:
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
				new1 = new Form(f2->kind, f2->index + f1->nlevel[2]);
				new1->num_safe = f2->num_safe;
				new1->nform[2] = f2->nform[2];
				f2->index += f1->nlevel[1];
				f2->connect1(0, f1->port(1));
				new1->connect1(0, f1->port(2));
				f1->connect1(0, f2->port(1));
				connect(f1, 1, f2, 1);
				connect(f1, 2, new1, 1);
				break;
			}
			break;

		case UNS_FAN1:
		case UNS_FAN2:
			if (f2->kind != TRIANGLE)
			{
				fan_int++;
			}
			unsafe++;
		case TRIANGLE:
			if (f2->kind == LAMBDA)
			{
				f1->num_safe = false;
			}
			switch (f2->kind)
			{
			case CONS:
			case EQ:
			case NOTEQ:
			case LEQ:
			case MEQ:
			case MORE:
			case LESS:
			case ADD:
			case SUB:
			case PROD:
			case DIV:
			case MOD:
			case AND:
			case OR:
			case IFELSE:
			case APP:
			case LAMBDA:
			case FAN:
			case TESTNIL1:
			case CAR1:
			case CDR1:
			case CONS1:
				new1 = new Form(f1->kind, f1->index);
				new1->nlevel[1] = f1->nlevel[1];
				new1->num_safe = f1->num_safe;

				f2->index += f1->nlevel[1];

				f2->connect1(0, f1->port(1));
				new1->connect1(0, f2->port(1));
				f1->connect1(0, f2->port(2));

				connect(f1, 1, f2, 2);
				connect(new1, 1, f2, 1);
				break;

			case TESTNIL:
			case CAR:
			case CDR:
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
			case NOT:
			case TRIANGLE:
			case LAMBDAUNB:
			case UNS_FAN1:
			case UNS_FAN2:
				f2->index += f1->nlevel[1];
				f2->connect1(0, f1->port(1));
				f1->connect1(0, f2->port(1));
				connect(f1, 1, f2, 1);
				break;
			}
			break;
		}
	}
}

//  The following function reduces the redex whose
//  interacting forms are passed as a parameter and the
//  second form is a INT, NIL, True or False;
void Form::reduce_form()
{
	Form *tmp;
	Form *f1 = this;
	switch (f1->kind)
	{
	case IFELSE:
		switch (f1->nport[0])
		{
		case T:
			eq++;
			f1->kind = CAR;
			f1->connect(0, f1->port(2));
			break;

		case F:
			eq++;
			f1->kind = CDR;
			f1->connect(0, f1->port(2));
			break;

		default:
			printf("--->   type error2\n");
			type_error = true;
			break;
		}
		break;

	case AND:
		switch (f1->nport[0])
		{
		case F:
			eq++;
			f1->bool_connect(1, F);
			f1->kind = ERASE;
			f1->connect1(0, f1->port(2));
			f1->del();
			if (option == 1)
			{
				gc.clean();
			}
			break;

		case T:
			eq++;
			::connect1(f1->nform[1],
					   f1->nport[1],
					   f1->nform[2],
					   f1->nport[2]);
			f1->release();
			break;
		default:
			printf("--->   type error3\n");
			type_error = true;
			break;
		}
		break;

	case OR:
		switch (f1->nport[0])
		{
		case T:
			eq++;
			f1->bool_connect(1, T);
			f1->kind = ERASE;
			f1->connect1(0, f1->port(2));
			f1->del();
			if (option == 1)
			{
				gc.clean();
			}
			break;

		case F:
			eq++;
			::connect1(f1->nform[1],
					   f1->nport[1],
					   f1->nform[2],
					   f1->nport[2]);
			f1->release();
			break;
		default:
			printf("--->   type error4\n");
			type_error = true;
			break;
		}
		break;

	case NOT:
		switch (f1->nport[0])
		{
		case T:
			eq++;
			f1->bool_connect(1, F);
			f1->release();
			break;
		case F:
			eq++;
			f1->bool_connect(1, T);
			f1->release();
			break;
		default:
			printf("--->   type error5\n");
			type_error = true;
			break;
		}
		break;

	case LESS:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->kind = LESS1;
			tmp = f1->nform[0];
			f1->connect1(0, f1->port(2));
			f1->nform[2] = tmp;
		}
		else
		{
			printf("--->   type error6\n");
			type_error = true;
		}
		break;

	case LESS1:
		if (f1->nport[0] == INT)
		{
			eq++;
			if (f1->nform[2] < f1->nform[0])
			{
				f1->bool_connect(1, T);
			}
			else
			{
				f1->bool_connect(1, F);
			}
			f1->release();
		}
		else
		{
			printf("--->   type error7\n");
			type_error = true;
		}
		break;

	case EQ:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->kind = EQ1;
			tmp = f1->nform[0];
			f1->connect1(0, f1->port(2));
			f1->nform[2] = tmp;
		}
		else
		{
			printf("--->   type error8\n");
			type_error = true;
		}
		break;

	case EQ1:
		if (f1->nport[0] == INT)
		{
			eq++;
			if (f1->nform[2] == f1->nform[0])
			{
				f1->bool_connect(1, T);
			}
			else
			{
				f1->bool_connect(1, F);
			}
			f1->release();
		}
		else
		{
			printf("--->   type error9\n");
			type_error = true;
		}
		break;

	case NOTEQ:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->kind = NOTEQ1;
			tmp = f1->nform[0];
			f1->connect1(0, f1->port(2));
			f1->nform[2] = tmp;
		}
		else
		{
			printf("--->   type error10\n");
			type_error = true;
		}
		break;

	case NOTEQ1:
		if (f1->nport[0] == INT)
		{
			eq++;
			if (f1->nform[2] != f1->nform[0])
			{
				f1->bool_connect(1, T);
			}
			else
			{
				f1->bool_connect(1, F);
			}
			f1->release();
		}
		else
		{
			printf("--->   type error11\n");
			type_error = true;
		}
		break;

	case LEQ:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->kind = LEQ1;
			tmp = f1->nform[0];
			f1->connect1(0, f1->port(2));
			f1->nform[2] = tmp;
		}
		else
		{
			printf("--->   type error12\n");
			type_error = true;
		}
		break;

	case LEQ1:
		if (f1->nport[0] == INT)
		{
			eq++;
			if (f1->nform[2] <= f1->nform[0])
			{
				f1->bool_connect(1, T);
			}
			else
			{
				f1->bool_connect(1, F);
			}
			f1->release();
		}
		else
		{
			printf("--->   type error13\n");
			type_error = true;
		}
		break;

	case MEQ:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->kind = MEQ1;
			tmp = f1->nform[0];
			f1->connect1(0, f1->port(2));
			f1->nform[2] = tmp;
		}
		else
		{
			printf("--->   type error14\n");
			type_error = true;
		}
		break;

	case MEQ1:
		if (f1->nport[0] == INT)
		{
			eq++;
			if (f1->nform[2] >= f1->nform[0])
			{
				f1->bool_connect(1, T);
			}
			else
			{
				f1->bool_connect(1, F);
			}
			f1->release();
		}
		else
		{
			printf("--->   type error15\n");
			type_error = true;
		}
		break;

	case MORE:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->kind = MORE1;
			tmp = f1->nform[0];
			f1->connect1(0, f1->port(2));
			f1->nform[2] = tmp;
		}
		else
		{
			printf("--->   type error16\n");
			type_error = true;
		}
		break;

	case MORE1:
		if (f1->nport[0] == INT)
		{
			eq++;
			if (f1->nform[2] > f1->nform[0])
			{
				f1->bool_connect(1, T);
			}
			else
			{
				f1->bool_connect(1, F);
			}
			f1->release();
		}
		else
		{
			printf("--->   type error17\n");
			type_error = true;
		}
		break;

	case ADD:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->kind = ADD1;
			tmp = f1->nform[0];
			f1->connect1(0, f1->port(2));
			f1->nform[2] = tmp;
		}
		else
		{
			printf("--->   type error18\n");
			type_error = true;
		}
		break;

	case ADD1:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->binop(add);
			f1->release();
		}
		else
		{
			printf("--->   type error19\n");
			type_error = true;
		}
		break;

	case SUB:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->kind = SUB1;
			tmp = f1->nform[0];
			f1->connect1(0, f1->port(2));
			f1->nform[2] = tmp;
		}
		else
		{
			printf("--->   type error20\n");
			type_error = true;
		}
		break;

	case SUB1:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->binop(sub);
			f1->release();
		}
		else
		{
			printf("--->   type error21\n");
			type_error = true;
		}
		break;

	case PROD:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->kind = PROD1;
			tmp = f1->nform[0];
			f1->connect1(0, f1->port(2));
			f1->nform[2] = tmp;
		}
		else
		{
			printf("--->   type error22\n");
			type_error = true;
		}
		break;

	case PROD1:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->binop(mult);
			f1->release();
		}
		else
		{
			printf("--->   type error23\n");
			type_error = true;
		}
		break;

	case DIV:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->kind = DIV1;
			tmp = f1->nform[0];
			f1->connect1(0, f1->port(2));
			f1->nform[2] = tmp;
		}
		else
		{
			printf("--->   type error24\n");
			type_error = true;
		}
		break;

	case DIV1:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->binop(div);
			f1->release();
		}
		else
		{
			printf("--->   type error25\n");
			type_error = true;
		}
		break;

	case MOD:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->kind = MOD1;
			tmp = f1->nform[0];
			f1->connect1(0, f1->port(2));
			f1->nform[2] = tmp;
		}
		else
		{
			printf("--->   type error26\n");
			type_error = true;
		}
		break;

	case MOD1:
		if (f1->nport[0] == INT)
		{
			eq++;
			f1->binop(rem);
			f1->release();
		}
		else
		{
			printf("--->   type error27\n");
			type_error = true;
		}
		break;

	case CDR:
		if (f1->nport[0] == NIL)
		{
			eq++;
			f1->bool_connect(1, NIL);
			f1->release();
		}
		else
		{
			printf("--->   type error\n");
			type_error = true;
		}
		break;

	case TESTNIL:
		if (f1->nport[0] == NIL)
		{
			eq++;
			f1->bool_connect(1, T);
			f1->release();
		}
		else
		{
			printf("--->   type error\n");
			type_error = true;
		}
		break;
	case CDR1:
		if (f1->nport[0] == NIL)
		{
			eq++;
			f1->bool_connect(1, NIL);
			f1->bool_connect(2, NIL);
			f1->release();
		}
		else
		{
			printf("--->   type error\n");
			type_error = true;
		}
		break;

	case TESTNIL1:
		if (f1->nport[0] == NIL)
		{
			eq++;
			f1->bool_connect(1, T);
			f1->bool_connect(2, NIL);
			f1->release();
		}
		else
		{
			printf("--->   type error\n");
			type_error = true;
		}
		break;

	case FAN:
		fan_int++;
		switch (f1->nport[0])
		{
		case NIL:
		case T:
		case F:
			f1->bool_connect(1, f1->nport[0]);
			f1->bool_connect(2, f1->nport[0]);
			f1->release();
			break;

		case INT:
			::int_connect(f1->nform[1],
						  f1->nport[1],
						  f1->nform[0],
						  f1->nport[0]);
			::int_connect(f1->nform[2],
						  f1->nport[2],
						  f1->nform[0],
						  f1->nport[0]);
			f1->release();
			break;
		}
		break;

	case TRIANGLE:
	case UNS_FAN1:
	case UNS_FAN2:
		if (f1->kind != TRIANGLE)
		{
			fan_int++;
		}
		switch (f1->nport[0])
		{
		case NIL:
		case T:
		case F:
			f1->bool_connect(1, f1->nport[0]);
			f1->release();
			break;

		case INT:
			::int_connect(f1->nform[1],
						  f1->nport[1],
						  f1->nform[0],
						  f1->nport[0]);
			f1->release();
			break;
		}
		break;
	default:
	{
		printf("--->   type error: constant in wrong position\n");
		type_error = true;
	}
	} // end switch
}

//  The following function looks for the leftmost outemost
//  redex, saving on the m_stack pointers to the form along
//  the main spine of the term.
Form *Form::lo_redex()
{
	Form *temp = this;
	Form *next = temp->nform[0];
	int p = temp->nport[0];

	while (p > 0)
	{
		while (temp->kind == TRIANGLE && temp->nlevel[1] == 0 && temp->num_safe)
		{
			::connect1(temp->nform[1], temp->nport[1],
					   temp->nform[0], temp->nport[0]);
			temp->release();
			temp = stack.pop();
		}
		if ((temp->kind == TRIANGLE || temp->kind == FAN ||
			 temp->kind == UNS_FAN1 || temp->kind == UNS_FAN2) &&
			(next->kind == TRIANGLE || next->kind == FAN) &&
			next->num_safe &&
			((is_connectable(next->nlevel[p]) && temp->index >= next->index &&
			  temp->index <= next->index + next->nlevel[p]) ||
			 (temp->num_safe &&
			  next->nlevel[p] < 0 && temp->index <= next->index &&
			  temp->index >= next->index + next->nlevel[p])))
		{
			switch (temp->kind)
			{
			case TRIANGLE:
			case UNS_FAN1:
			case UNS_FAN2:
				counter++;
				optim++;
				switch (next->kind)
				{
				case TRIANGLE:
					next->nlevel[1] += temp->nlevel[1];
					if (next->nlevel[1] == 0)
					{
						::connect1(temp->nform[1],
								   temp->nport[1],
								   next->nform[0],
								   next->nport[0]);
						temp->release();
						next->release();
						temp = stack.pop();
					}
					else
					{
						::connect(temp->nform[1],
								  temp->nport[1],
								  next, 1);
						temp->release();
						temp = stack.pop();
					}
					break;

				case FAN:
					next->nlevel[p] += temp->nlevel[1];
					::connect(temp->nform[1],
							  temp->nport[1],
							  next, p);
					temp->release();
					temp = stack.pop();
					break;
				}
				break;

			case FAN:
				switch (next->kind)
				{
				case TRIANGLE:
					temp->num_safe = true;
					temp->index = next->index;
					temp->nlevel[1] += next->nlevel[1];
					temp->nlevel[2] += next->nlevel[1];

					temp->connect1(0, next->port(0));
					next->release();
					temp = stack.pop();
					counter++;
					optim++;
					break;

				case FAN:
					stack.push(temp);
					temp = next;
					break;
				}
				break;
			}
		}
		else
		{
			stack.push(temp);
			temp = next;
		}
		next = temp->nform[0];
		p = temp->nport[0];
	}
	return temp;
}
