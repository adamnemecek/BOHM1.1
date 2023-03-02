 /***************************************************************/
 /* This module is the YACC_specification of the parser.        */
 /* All inputs must be terminated by a double simicolon         */
 /* An input is either a term or a directive.                   */
 /* A term can be a global definition of the kind               */
 /*  - def <identifier> = <expr> in <expr>                      */
 /* or a single expression.                                     */
 /*                                                             */
 /* Expressions are syntactically defined by the following      */
 /* grammar:                                                    */
 /*                                                             */
 /*  <expr> ::= <expr0>                                         */
 /*     |  <expr> < <expr>                                      */
 /*     |  <expr> == <expr>                                     */
 /*     |  <expr> > <expr>                                      */
 /*     |  <expr> <= <expr>                                     */
 /*     |  <expr> >= <expr>                                     */
 /*     |  <expr> <> <expr>                                     */
 /*     |  <expr> + <expr>                                      */
 /*     |  <expr> - <expr>                                      */
 /*     |  <expr> * <expr>                                      */
 /*     |  <expr> div <expr>                                    */
 /*     |  <expr> mod <expr>                                    */
 /*     |  - <expr>   	                                        */
 /*     |  <expr> < <expr>                                      */
 /*     |  <expr> == <expr>                                     */
 /*     |  <expr> > <expr>                                      */
 /*     |  <expr> <= <expr>                                     */
 /*     |  <expr> >= <expr>                                     */
 /*     |  <expr> <> <expr>                                     */
 /*     |  <expr> + <expr>                                      */
 /*     |  <expr> - <expr>                                      */
 /*     |  <expr> * <expr>                                      */
 /*     |  <expr> div <expr>                                    */
 /*     |  <expr> mod <expr>                                    */
 /*                                                             */
 /*  <expr0> ::= true                                           */
 /*     |  false                                                */
 /*     |  <num\_const>                                         */
 /*     |  <identifier>                                         */
 /*     |  (<applist>)                                          */
 /*     |  \ <pattern> . <expr>                                 */
 /*     |  let <identifier> = <expr> in  <expr>                 */
 /*     |  letrec <identifier> = <expr>                         */
 /*     |  if <expr> then  <expr> else  <expr>                  */
 /*     |  <expr> and  <expr>                                   */
 /*     |  <expr> or  <expr>                                    */
 /*     |  not <expr>                                           */
 /*     |  <list>                                               */
 /*     |  cons (<expr>,<expr>)                                 */ 
 /*     |  head (<expr>)                                        */
 /*     |  tail (<expr>)                                        */
 /*     |  isnil (<expr>)                                       */
 /*  <list> ::= nil                                             */
 /*     | [<exprlist>]                                          */
 /*  <exprlist> ::=        (* empty *)                          */
 /*     | <expr>                                                */
 /*     | <expr>,<exprlist>                                     */
 /*     | <expr>|<expr>                                         */
 /*  <applist> ::= <expr>                                       */
 /*     |  <applist> <expr0>                                    */
 /*     |  <expr>,<comlist>                                     */
 /*  <comlist> ::= <expr>                                       */
 /*     |  <expr>,<comlist>                                     */
 /*  <pattern> ::= <identifier>                                 */
 /*     |  cons(<pattern>,<pattern>)                            */
 /*     |  (<comlistpat>)                                       */
 /*  <comlistpat> ::= <pattern>                                 */
 /*     |  <pattern>,<comlistpat>                               */
 /*                                                             */
 /* The <expr0> nonterminal represents an expression which      */
 /* cannot start with a minus sign.                             */
 /* (comlist) is represents a tuple; tuples are                 */
 /* currently implemented as non-nil-terminated lists.          */
 /*                                                             */
 /* Each directive must be preceded by the symbol "#".          */
 /* There are seven directives:                                 */
 /*  - inspect  to enter inspection mode (the name of a         */
 /*             globally defined term to inspect can be passed  */
 /*             as a parameter; otherwise, inspection of the    */
 /*             last compiled term is assumed).                 */
 /*  - save	to save the graph of a term in a file.		*/
 /*		The first identifier is the file name, the 	*/
 /*		second is the term (if the latter does not 	*/
 /*             exist, the last term is taken).                 */
 /*  - quit     to exit from the compiler.                      */
 /*  - load     to load an external file; the file name must    */
 /*             be specified as a normal idenitifier            */
 /*  - garbage  to invoke gargabe collection                    */
 /*  - option   to choose a grabge collection strategy          */
 /*  - info     to get general informations on the computation  */
 /***************************************************************/

 /***************************************************************/
 /* 1. Inclusion of header files.				*/
 /***************************************************************/

%{
#include "bohm.h"

%}


 /***************************************************************/
 /* 2. Inclusion of declarations that are being imported.	*/
 /***************************************************************/

%{
#ifdef __cplusplus
extern "C" {
#endif

int yylex();
int yyerror(const char *msg);

#ifdef __cplusplus
}
#endif
%}

 /***************************************************************/
 /* 3. Definitions of variables to be exported.			*/
 /***************************************************************/

%{
bool			quit,
			       /* flag indicating quit request */
			loading_mode;
			       /* flag indicating if parsing is */
			       /* done after a load directive */
char                    *include_file;
Form                    *lastinputterm,
			       /* pointer to the root of the */
			       /* term in input */
                        *current_pos;
 			       /* for inspection mode */
%}

 /***************************************************************/
 /* 4. Definitions strictly local to the module.                */
 /***************************************************************/

%{
#define UNBOUND_VARIABLE "scoping error: undefined variable"

int                    app_nesting_depth;
Pattern                *pattmp;

static bool defined(StBucket	*st);
%}

 /***************************************************************/
 /* 5. Definition of grammar symbols attributes.		*/
 /***************************************************************/

%union	{
		/* pointer to a symbol */
		/* table bucket */
		/* numerical constant */
		StBucket	*st_bucket;
		
		int		num_const;

		/* graph representation */
		/* of the term */
		Term		*term;
	
		/* root_form  */
		Form		*root;

		char            *astring;
		Pattern         *pattern;
	}


 /***************************************************************/
 /* 6. Definitions of tokens.					*/
 /***************************************************************/

%token				EXPRDELIM      257
%token	<st_bucket>		ID	       258
%token	<num_const>		NUMCONST       259
%token				EQUAL          260
%token				LEQUAL         261
%token				MEQUAL         262
%token				NOTEQUAL       263
%token				LETKW	       400
%token				INKW           401
%token                          INSPECTKW      402
%token                          QUITKW         403
%token                          LOADKW         404
%token				LETRECKW       405
%token				TRUEKW         406
%token				FALSEKW        407
%token				IFKW           408
%token				THENKW         409
%token				ELSEKW         410
%token				ANDKW          411
%token				ORKW           412
%token				NOTKW          413
%token				DIVKW          414
%token				MODKW          415
%token				CONSKW         416
%token			        HEADKW         417
%token				TAILKW         418
%token				TESTNILKW      419
%token				DEFKW          420
%token				NILKW          422
%token				GARBAGEKW      423
%token				OPTIONKW       424
%token				INFOKW         425
%token				SAVEKW         426
%token				TRAVELKW       427
%token <astring>                ASTRING        428


 /***************************************************************/
 /* 7. Association of attributes with nonterminals.		*/
 /***************************************************************/

%type	<root>		        input
%type	<root>		        directive
%type	<root>		        arg
%type	<root>		        term
%type	<root>		        global_decl
%type	<term>			expr
%type   <term>                  expr0
%type	<term>			applist
%type	<term>			comlist
%type	<term>			exprlist
%type	<term>			list
%type   <pattern>               pattern
%type   <pattern>               comlistpat
%type   <astring>               optstring
%type   <num_const>             optint


 /***************************************************************/
 /* 8. Precedence and associativity of operators.		*/
 /***************************************************************/

%nonassoc ELSEKW INKW '.' '='
%left ANDKW ORKW
%left NOTKW
%left '<' '>' EQUAL LEQUAL MEQUAL NOTEQUAL
%left '+' '-'
%left '*' DIVKW MODKW
%left NEG

 /***************************************************************/
 /* 9. Definition of the start symbol.				*/
 /***************************************************************/


%start			input


 /***************************************************************/
 /* 10. Grammar rules.						*/
 /***************************************************************/

%%

input           :      directive
		|               {
				   app_nesting_depth = 0;
				   destroyer.destroy();
				   lastinputterm=NULL;
				}
		       term
				{
				   $$ = $2;
				}
		|        
				{
				   printf("no more input");
				   quit = true;
				   YYACCEPT;
				}
				; 

directive       :      '#' INSPECTKW arg EXPRDELIM
				{
				   if ($3 == NULL) $3=lastinputterm;
				   $$ = $3;
				   $3->inspect_driver();
				   YYACCEPT;
				}
                |      '#' TRAVELKW arg NUMCONST EXPRDELIM
				{
				   if ($3 == NULL) $3=current_pos;
				   current_pos = $3->inspect($4);
				   YYACCEPT;
				}
		|      '#' QUITKW EXPRDELIM
				{
				   quit = true;
				   YYACCEPT;
				}
		|      '#' LOADKW ASTRING EXPRDELIM
				{
				  include_file = $3;
				  loading_mode = true;
				  printf("%s", include_file);
				  YYACCEPT;
				}
		|       '#' GARBAGEKW EXPRDELIM
				{
				   user();
				   YYACCEPT;
				}
		|       '#' OPTIONKW optint EXPRDELIM
				{
				   menu($3);
				   YYACCEPT;
				}
		|       '#' INFOKW optstring EXPRDELIM
				{
				   info($3);
				   YYACCEPT;
				}
		|       '#' SAVEKW ASTRING EXPRDELIM
				{
				   lastinputterm->save($3,NULL);
                                   free($3);
				   YYACCEPT;
				}
		|       '#' SAVEKW ASTRING ID EXPRDELIM
				{
				   if (defined($4))
				      $4->curr_binding->root->save($3,$4->id);
				   else
				      {
					 signal_error(UNBOUND_VARIABLE);
					 YYACCEPT;
				      }
                                   free($3);
				   YYACCEPT;
				}
		;

arg             :
				{
				   $$ = NULL;
				}
		|       ID
				{
				   if (defined($1))
				      $$ = $1->curr_binding->root;
				   else
				      {
					 signal_error(UNBOUND_VARIABLE);
					 YYACCEPT;
				      }
				 }
		;

optstring       :
				{
				   $$ = "";
				}
                |       ASTRING
                                {
				   $$ = $1;
				}

optint          :
				{
				   $$ = -1;
				}
                |       NUMCONST
                                {
				   $$ = $1;
				}

term            :       expr EXPRDELIM
				{
				  lastinputterm = $1->close(0);
				  current_pos = lastinputterm;
				  $$ = lastinputterm;
				  $$->reduce_term();
				  YYACCEPT;
				}
		|       global_decl EXPRDELIM
				{
				  destroyer.no_destroy();
				  YYACCEPT;
				}
		;



global_decl	:    DEFKW ID '='
				{
				  app_nesting_depth++;
				}
		     expr
				{
				  app_nesting_depth--;
				  lastinputterm = $5->close(1);
				  $$ = lastinputterm;
				  $2->create_variable_binding($$);
				}
		;

expr            :       expr0
                                {
                                  $$ = $1;
                                }
		|	'-' expr %prec NEG
				{
		 		  $$ = Term::neg(app_nesting_depth, $2);
				}
		 |	expr ANDKW expr
				{
				  $$ = Term::and_(app_nesting_depth,
						       $1,$3);
				}
		 |	expr ORKW expr
				{
				  $$ = Term::or_(app_nesting_depth,
						       $1,$3);
				}
		 |	expr '<' expr
		 		{
		 		  $$ = Term::relop(app_nesting_depth,
		 		  		       $1,$3,LESS);
		 		}
		 |	expr EQUAL expr
		 		{
		 		  $$ = Term::relop(app_nesting_depth,
		 		  		       $1,$3,EQ);
		 		}
		 |	expr NOTEQUAL expr
		 		{
				  $$ = Term::relop(app_nesting_depth,
		 		  		       $1,$3,NOTEQ);
		 		}
		 |	expr '>' expr
		 		{
		 		  $$ = Term::relop(app_nesting_depth,
		 		  		       $1,$3,MORE);
				}
		 |	expr LEQUAL expr
		 		{
		 		  $$ = Term::relop(app_nesting_depth,
		 		  		       $1,$3,LEQ);
		 		}
		 |	expr MEQUAL expr
		 		{
		 		  $$ = Term::relop(app_nesting_depth,
		 		  		       $1,$3,MEQ);
		 		}
		 |	expr '+' expr
				{
				  $$ = Term::matterm(app_nesting_depth,
		 		  		       $1,$3,ADD);
				}
		 |	expr '-' expr
				{
		 		  $$ = Term::matterm(app_nesting_depth,
		 		  		       $1,$3,SUB);
				}
		 |	expr '*' expr
				{
		 		  $$ = Term::matterm(app_nesting_depth,
		 		  		       $1,$3,PROD);
				}
		 |	expr DIVKW expr
				{
		 		  $$ = Term::matterm(app_nesting_depth,
		 		  		       $1,$3,DIV);
				}
		 |	expr MODKW expr
				{
				  $$ = Term::matterm(app_nesting_depth,
		 		  		       $1,$3,MOD);
		 		}
                ;

expr0           : 	TRUEKW
				{
				  $$ = Term::true_(app_nesting_depth);
				}
		| 	FALSEKW
				{
				  $$ = Term::false_(app_nesting_depth);
				}
		| 	NUMCONST
				{
				  $$ = Term::int_(app_nesting_depth,$1);
				}
		|       ID
				{
				  if (defined($1))
				     $$ = Term::var(app_nesting_depth,$1);
				  else
				     {
					signal_error(UNBOUND_VARIABLE);
					YYERROR;
				     }
				}
		|       '(' applist ')'
				{
				  $$ = $2;
				}
		|       '\\' 
				{
				  st.push_local_env();
                                  app_nesting_depth++;
				}
			pattern 
                                {
                                  app_nesting_depth--;
                                }
                        '.' expr
				{
                                  pattmp=$3;
                                  $$ = 
                                    Term::plambda(app_nesting_depth,$3,$6);
                                  delete pattmp;
				  st.pop_local_env();
				}
		|       LETKW ID '='
				{
				  app_nesting_depth++;
				}
			expr INKW
				{
				  app_nesting_depth--;
				  st.push_local_env();
				  $2->create_variable_binding(NULL);
				}
			expr
				{
				  $$ = Term::let_in(app_nesting_depth,
						      $2,$5,$8);
				  st.pop_local_env();
				}
		 |	LETRECKW ID '='
				{
				  st.push_local_env();
				  $2->create_variable_binding(NULL);
				  app_nesting_depth++;
				 }
			expr
				{
				  $$ = Term::mu(--app_nesting_depth,
						     $2,$5);
				  st.pop_local_env();
				 }
		 |	IFKW expr THENKW expr ELSEKW expr
				{
				  $$ = Term::ifelse(app_nesting_depth,
						       $2,$4,$6);
				}
		 |	list
				{
				  $$ = $1;
				}

		 |	NOTKW expr
		 		{
				  $$ = Term::not_(app_nesting_depth,
		 		  		       $2);
				}
		 |	CONSKW '(' expr ',' expr ')'
				{
		 		  $$ = Term::list(app_nesting_depth,
		 		  		       $3,$5);
		 		}	
		 |	HEADKW '(' expr ')'
				{
		 		  $$ = Term::car(app_nesting_depth,
		 		  		       $3);
		 		}
		 |	TAILKW '(' expr ')'
				{
				  $$ = Term::cdr(app_nesting_depth,
		 		  		       $3);
		 		}
		 |	TESTNILKW '(' expr ')'
				{
		 		  $$ = Term::testnil(app_nesting_depth,
		 		  		       $3);
				}
                 ;

list		 :	NILKW
				{
				  $$ = Term::nillist(app_nesting_depth);
				}
		 |      '[' ']'
                                {
                                  $$ = Term::nillist(app_nesting_depth);
				}
		 |	'[' exprlist
				{
				  $$ = $2;
				}
		 ;
exprlist	:	expr ']'
				{
				  $$ = Term::list(app_nesting_depth,
						$1,NULL);
				}
		|	expr ',' exprlist
				{
				  $$ = Term::list(app_nesting_depth,
						$1,$3);
				}
		|       expr '|' expr ']'
				{
				  $$ = Term::list(app_nesting_depth,
						 $1,$3);
				}
		;

applist         :       expr
                |       expr ',' comlist
                                {
		 		  $$ = Term::list(app_nesting_depth,
		 		  		       $1,$3);
                                }
		|       applist
				{
				  app_nesting_depth++;
				}
			expr0
				{ app_nesting_depth--;
				  $$ = Term::app(app_nesting_depth,$1,$3);
				}
		;


comlist         :       expr
                                {
                                  $$=$1;
                                }
                |       expr ',' comlist
                                {
		 		  $$ = Term::list(app_nesting_depth,
		 		  		       $1,$3);
                                }


comlistpat      :       pattern
                                {
                                  $$=$1;
                                }
                |       pattern ',' comlistpat
                                {
                                  pattmp=(Pattern *)malloc(sizeof(Pattern));
                                  pattmp->term=
                                    Term::list1(app_nesting_depth,
                                              $1->term,$3->term);
                                  pattmp->var_list=
                                    $1->var_list->merge($3->var_list);
                                  free($1);
                                  free($3);
                                  $$=pattmp;
                                }

pattern         :       CONSKW '(' pattern ',' pattern ')'
                                {
                                  pattmp=(Pattern *)malloc(sizeof(Pattern));
                                  pattmp->term=
                                    Term::list1(app_nesting_depth,
                                              $3->term,$5->term);
                                  pattmp->var_list=
                                    mergevarlist($3->var_list,$5->var_list);
                                  free($3);
                                  free($5);
                                  $$=pattmp;
                                }
                |       NILKW
                                {
                                  pattmp=(Pattern *)malloc(sizeof(Pattern));
                                  pattmp->term=Term::nillist(app_nesting_depth);
                                  pattmp->var_list=NULL;
                                  $$=pattmp;
                                }
                |       '(' comlistpat ')'
                                {
                                  $$=$2;
                                }
                |       ID
                                {
                                  pattmp=(Pattern *)malloc(sizeof(Pattern));
                                  pattmp->term=
                                    Term::void_(app_nesting_depth);
                                  $1->create_variable_binding(NULL);
                                  pattmp->var_list=
                                    makevarlist($1,pattmp->term);
                                  $$=pattmp;
                                }

term    	:	error  EXPRDELIM
                                {
                                  error_detected = true;
                                  yyerrok;
                                  YYACCEPT;
				}
		;

%%

/* The following function checks if an identifier has been */
/* previously declared */
static bool defined(
	StBucket	*st
)
			      /* pointer to the bucket for the */
			      /* identifier */
{
        return st->curr_binding != NULL;
}
