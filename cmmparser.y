%{
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
extern int yylex();
NBlock *programBlock;
void yyerror (char const *s) {
		printf("Error: %s\n", s);
}
%}

%union {
	Node *node;
	NBlock *block;
	NExpression *expr;
	NStatement *stmt;
	NIdentifier *ident;
	NVariableDeclaration *var_decl;
	std::vector<NVariableDeclaration*> *varvec;
	std::vector<NExpression*> *exprvec;
  std::vector<NStatement*> *stmtvec;
	std::string *string;
	int token;
}

%token <token> T_lt T_gt T_lte T_gte T_eql T_neq 
%token <string> T_intconst T_identifier T_void T_int 
%token <token> T_while T_if T_else T_return
%token <token> T_add T_minus T_times T_divide T_assign
%token <token> T_lparen T_rparen T_lbracket T_rbracket T_lbrace T_rbrace T_semicolon T_comma

%type <ident> type_specifier var
%type <varvec> params params_list
%type <exprvec> statement_list arg_list args
%type <stmtvec> local_declarations
%type <stmt> declaration var_declaration fun_declaration expression_stmt  param
%type <expr>  expression simple_expression additive_expression term factor call statement statement1 iteration_stmt iteration_stmt1 return_stmt 
%type <block> program declaration_list compound_stmt
%type <token> relop addop mulop

%left '+' '-'
%left '*' '/'

%start program

%%

program : declaration_list
          { programBlock = $1; }
        ;

declaration_list : declaration_list declaration
                   { $1->statements.push_back($2); }
                 | declaration
                   { $$ = new NBlock(); $$->statements.push_back($1); }
                 ;

declaration : var_declaration
              { $$ = $1; }
            | fun_declaration
              { $$ = $1; }
            ;


var_declaration : type_specifier  T_identifier ';'
                  { $$ = new NVariableDeclaration(*$1, *$2); }
                | type_specifier  T_identifier '[' T_intconst ']' ';'
                  { $$ = new NVariableDeclaration(*$1, *$2, *$4); }
                ;

type_specifier :  T_int
                  { $$ = new NIdentifier(*$1); delete $1; }
               |  T_void
                  { $$ = new NIdentifier(*$1); delete $1; }
               ;

fun_declaration : type_specifier T_identifier '(' params ')' compound_stmt
                  { $$ = new NFunctionDeclaration(*$1, *$2, *$4, *$6)}
                ;

params : params_list
         { $$ = $1 }
       |  T_void
         { $$ = NULL; }
       ;

params_list : params_list ',' param
              { $1->push_back($<var_decl>3); }
            | param
              { $$ = new VariableList(); $$->push_back($<var_decl>1); }
           ;

param : type_specifier  T_identifier
        { $$ = new NVariableDeclaration(*$1, *$2); }
      | type_specifier  T_identifier '['']'
        { $$ = new NVariableDeclaration(*$1, *$2); }
      ;

compound_stmt : '{' local_declarations statement_list '}'
                { $$ = new NCompoundStatementDeclaration(*$2, *$3); $2 = new StatementList(); $3 = new ExpressionList() }
              ;

local_declarations : local_declarations var_declaration
                     { $1->push_back($2); }
                   | empty
                     { $$ = NULL; }
                   ;

statement_list : statement_list statement
                 { $1->push_back($2); }
               | empty
                 { $$ = NULL; }
               ;

statement : T_if '(' expression ')' statement1  T_else statement
            { $$ = new NIfStatement(*$3, *$5, *$7); }
          | T_if '(' expression ')' statement
            { $$ = new NIfStatement(*$3, *$5); }
          | expression_stmt
            { $$ = $<expr>1; }
          | compound_stmt
            { $$ = $1; }
          | iteration_stmt
            { $$ = $<expr>1; }
          | return_stmt
            { $$ = $<expr>1; }
          ;

statement1 : T_if '(' expression ')' statement1  T_else statement1
             { $$ = new NIfStatement(*$3, *$5, *$7); }
           | expression_stmt
             { $$ = $<expr>1; }
           | compound_stmt
             { $$ = $1; }
           | iteration_stmt1
             { $$ = $<expr>1; }
           | return_stmt
             { $$ = $<expr>1; }
           ;

        
expression_stmt : expression ';'
                  { $$ = $<stmt>1; }
                | ';'
                  { $$ = NULL; }
                ;



iteration_stmt :  T_while '(' expression ')' statement
                  { $$ = new NIterationStatement(*$3, *$5); }
               ;

iteration_stmt1 :  T_while '(' expression ')' statement1
                  { $$ = new NIterationStatement(*$3, *$5); }
               ;

return_stmt :  T_return ';'
               { $$ = new NReturnStatement(); }
            |  T_return expression ';'
               { $$ = new NReturnStatement(*$2); }
            ;

expression : var '=' expression
             { $$ = new NAssignment(*$1, *$3); }
           | simple_expression
             { $$ = $1; }
           ;

var :  T_identifier
       { $$ = new NIdentifier(*$1); }
    |  T_identifier '[' expression ']'
       { $$ = new NIdentifier(*$1, *$3); }
    ;

simple_expression : additive_expression relop additive_expression
                    { $$ = new NComparisonExpression(*$1, *$3, $2); }
                  | additive_expression
                    { $$ = $1; }
                  ;

relop : T_lte
      | T_lt
      | T_gt
      | T_gte
      | T_eql
      | T_neq
      ;


additive_expression : additive_expression addop term
                      { $$ = new NBinaryOperator(*$1, *$3, $2); }
                    | term
                      { $$ = $1; }
                    ;

addop : T_add
      | T_minus
      ;

term : term mulop factor
       { $$ = new NBinaryOperator(*$1, *$3, $2);}
     | factor
       { $$ = $1; }
     ;

mulop : T_times
      | T_divide
      ;

factor : '(' expression ')'
         { $$ = $2; }
       | var
         { $$ = $1; }
       | call
         { $$ = $1; }
       | T_intconst
         { $$ = $<expr>1; }
       ;

call :  T_identifier '(' args ')'
        { $$ = new NCallNode(*$1, *$3); }
     ;

args : arg_list
       { $$ = $1; }
     | empty
       { $$ = NULL; }
     ;

arg_list : arg_list ',' expression
           { $1->push_back($3); }
         | expression
           { $$ = new ExpressionList(); $$->push_back($1); }
         ;

empty : /* empty */
      ;
%%

int main() {
    return yyparse();
}