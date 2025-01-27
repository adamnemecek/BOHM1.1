//***********************************************************************
// This module implements the garbage collector.
// It works by local interactions, propagating all the erase-operators
// in the graph. Each erase-operator terminates its travelling either by
// annihilating against another one, or being "absorbed" by a different
// operator, such as "lambdaunb", "uns_fan1", "uns_fan2".
// When the garbage collector is activated, it finds all erase operators
// looking into a list in which they are inserted when created.
// The following functions are external:
// - init_garbage: it initializes the erase-list inserting the first
//		   node.
// - ins_del(): insert a new erase operator at the head of a list
//              to be scanned when the G.C. is activated.
// - clean(): it activates the G.C. by scanning the erase-list and by
//            propagating them in the graph. It uses the local function
//            "garbage()" which propagates a single node and
//            inserts in the erases list new operators originated by
//            duplication rules during travelling.
// - user():  it only calls the previous function and prints some data
//            when the user digits the directive"#garbage".
// The following functions are internal:
// - garbage(): It performs the propagation of a single erase node by
//              applicating garbage rules.
//***********************************************************************

//***********************************************************************
// 1. Inclusion of header files.
//***********************************************************************

#include "bohm.h"

//***********************************************************************
// 2. Inclusion of declarations that are being imported.
//***********************************************************************

//***********************************************************************
// 3. Declaration of names strictly local to the module.
//***********************************************************************

// constants concerning garbage
#define EXISTENT 3
#define NOTEXISTENT 4

// static long unsigned er_count; // counter for erasing operations
// static long unsigned cl_count; // counter for clean() calls
// static clock_t usr_garb_time;
// static clock_t sys_garb_time;

//***********************************************************************
// 4. Definitions of variables to be exported.
//***********************************************************************

// Form *del_head = NULL; // head of erases list

//***********************************************************************
// 5. Definitions of functions to be exported.
//***********************************************************************

// The following function initializes the erase-list inserting
// the first node.
Garbage::Garbage()
{
	del_head = (Form *)malloc_da(sizeof(Form));
	del_head->nform[1] = NULL;
}

// The following function insert a new erase operator at the
// head of a list to be scanned when the G.C. is activated.
void Garbage::del(Form *form)
{
	form->index = EXISTENT;
	form->nform[1] = del_head->nform[1];
	del_head->nform[1] = form;
}

// The following function activates the G.C. by scanning the
// erase-list and by propagating them in the graph. It uses
// the local function "garbage()" which propagates a single
// node and inserts in the erases list new operators
// originated by duplication rules during travelling.
void Garbage::clean()
{
	Form *q;
	struct tms partial_time, final_time;
	if (seegarb)
	{
		times(&partial_time);
	}
	cl_count++;
	while ((q = del_head->nform[1]) != NULL)
	{
		del_head->nform[1] = q->nform[1];
		if (q->index == NOTEXISTENT)
		{
			q->release();
		}
		else
		{
			this->garbage(q);
		}
	}
	if (seegarb)
	{
		times(&final_time);
		usr_garb_time += final_time.tms_utime - partial_time.tms_utime;
		usr_garb_time += final_time.tms_stime - partial_time.tms_stime;
	}
}

// The following function it only calls the previous function
// and prints some data when the user digits the directive
// "#garbage".
void user(void)
{
	printf("*****************************************************\n");
	printf("Initial number of nodes %u\n", num_nodes);
	printf("Please wait . . .\n");
	gc.clean();
	printf("Final number of nodes %u\n", num_nodes);
	printf("*****************************************************\n");
}

//*********************************************** ***********************
// 6. Definitions of functions strictly local to the module.
//***********************************************************************

// The following function performs the propagation of a single
// erase node by applicating garbage rules.
void Garbage::garbage(Form *f1)
{

	bool end = false;
	Form *newform;
	int p1, p2;
	auto [nextform, nextport] = f1->port(0);
	f1->release();
	while (!end)
	{
		Form *form = nextform;
		int port = nextport;
		// we have found a variable
		if (port < 0)
		{
			er_count++;
			end = true;
			continue;
		}

		switch (form->kind)
		{

		case TESTNIL1:
		case CAR1:
		case CDR1:
			if (port == 0)
			{
				er_count++;
				form->kind = ERASE;
				nextform = form->nform[2];
				nextport = form->nport[2];
				form->connect1(0, form->port(1));
				this->del(form);
			}
			else
			{
				if (port == 1)
				{
					if (form->nlevel[2] == 0)
					{
						::connect1(form->nform[2],
								   form->nport[2],
								   form->nform[0],
								   form->nport[0]);
						form->release();
					}
					else
					{
						form->kind = TRIANGLE;
						form->connect1(1, form->port(2));
						form->nlevel[1] = form->nlevel[2];
						form->index -= 1;
					}
				}
				else
				{
					if (form->nlevel[1] == 0)
					{
						form->kind = propagate_kind(form->kind);
					}
					else
					{
						form->kind = TRIANGLE;
						newform = new Form(propagate_kind(form->kind), form->index);
						newform->connect1(0, form->port(0));
						::connect(newform, 1, form, 0);
						form->index -= 1;
					}
				}
				end = true;
			}
			break;

		case CONS1:
			if (port == 0)
			{
				er_count++;
				nextform = form->nform[2];
				nextport = form->nport[2];
				form->kind = ERASE;
				form->connect1(0, form->port(1));
				this->del(form);
			}
			else
			{
				if (port == 1)
				{
					form->kind = CDR;
					::connect1(form->nform[2], form->nport[2], form, 1);
				}
				else
				{
					form->kind = CAR;
				}
				end = true;
			}
			break;

		case FAN:
			if (port == 0)
			{
				er_count++;
				nextform = form->nform[2];
				nextport = form->nport[2];
				form->kind = ERASE;
				form->connect1(0, form->port(1));
				this->del(form);
			}
			else
			{
				if (form->num_safe)
				{
					if (port == 1)
					{
						form->kind = TRIANGLE;
						form->connect1(1, form->port(2));
						form->nlevel[1] = form->nlevel[2];
					}
					else
					{
						form->kind = TRIANGLE;
					}
				}
				else if (port == 1)
				{
					form->kind = UNS_FAN1;
					form->connect1(1, form->port(2));
					form->nlevel[1] = form->nlevel[2];
				}
				else
				{
					form->kind = UNS_FAN2;
				}
				end = true;
			}
			break;

		case TRIANGLE:
		case LAMBDAUNB:
		case TESTNIL:
		case CAR:
		case CDR:
		case NOT:
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
		case UNS_FAN1:
		case UNS_FAN2:
			er_count++;
			if (port == 0)
			{
				nextform = form->nform[1];
				nextport = form->nport[1];
			}
			else
			{
				nextform = form->nform[0];
				nextport = form->nport[0];
			}
			form->release();
			break;

		case APP:
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
			er_count++;
			switch (port)
			{
			case 0:
				p1 = 1;
				p2 = 2;
				break;
			case 1:
				p1 = 0;
				p2 = 2;
				break;
			case 2:
				p1 = 0;
				p2 = 1;
				break;
			}
			nextform = form->nform[p2];
			nextport = form->nport[p2];
			form->kind = ERASE;
			form->connect1(0, form->port(p1));
			this->del(form);
			break;

		case LAMBDA:
			if (port != 2)
			{
				er_count++;
				nextform = form->nform[!port];
				nextport = form->nport[!port];
				form->kind = ERASE;
				form->connect1(0, form->port(2));
				this->del(form);
			}
			else
			{
				form->kind = LAMBDAUNB;
				end = true;
			}
			break;

		case ERASE:
			er_count++;
			form->index = NOTEXISTENT;
			end = true;
			break;

		default:
			printf("Error: form %d\n", (f1->nform[0])->kind);
			exit(1);
			break;
		}
	}
}

void Garbage::reset(void)
{
	er_count = 0;
	cl_count = 0;
	usr_garb_time = 0;
	sys_garb_time = 0;
}

void Garbage::show_stats(bool seetime)
{
	printf("Total number of garbage calls      %lu\n", cl_count);
	printf("Total number of garbage operations %lu\n", er_count);
	if (seetime)
	{
		printf("Garbage collection done in %.2f:usr %.2f:sys seconds\n",
			   (double)usr_garb_time / 60, (double)sys_garb_time / 60);
		printf("*****************************************************\n");
	}
}

void Form::del()
{
	gc.del(this);
}