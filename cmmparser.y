%{
#include <stdio.h>
#include <stdlib.h>
void yyerror (char const *s) {
  fprintf (stderr, "%s\n", s);
}
#define YYSTYPE char *
%}

%token T_lte T_gte T_eql T_neq T_intconst T_identifier T_void T_int T_while T_if T_else T_return T_comment

%left '+' '-'
%left '*' '/'

%%



program : declaration_list
        ;

declaration_list : declaration_list declaration
                 | declaration
                 ;

declaration : var_declaration
            | fun_declaration
            ;


var_declaration : type_specifier  T_identifier ';'
                | type_specifier  T_identifier '[' T_intconst ']' ';'
                ;

type_specifier :  T_int
               |  T_void
               ;

fun_declaration : type_specifier T_identifier '(' params ')'
                | compound_stmt
                ;

params : params_list
       |  T_void
       ;

params_list : params_list ',' param
           | param
           ;

param : type_specifier  T_identifier
      | type_specifier  T_identifier '['']'
      ;

compound_stmt : '{' local_declarations statement_list '}'
              ;

local_declarations : local_declarations var_declaration
                   | empty
                   ;

statement_list : statement_list statement
               | empty
               ;

statement :
          T_if '(' expression ')' statement1  T_else statement
          | T_if '(' expression ')' statement
          | expression_stmt
          | compound_stmt
          | iteration_stmt
          | return_stmt
          ;

statement1 :
          T_if '(' expression ')' statement1  T_else statement1
          | expression_stmt
          | compound_stmt
          | iteration_stmt1
          | return_stmt
          ;

        
expression_stmt : expression ';'
                | ';'
                ;


iteration_stmt :  T_while '(' expression ')' statement
               ;

iteration_stmt1 :  T_while '(' expression ')' statement1
               ;

return_stmt :  T_return ';'
            |  T_return expression ';'
            ;

expression : var '=' expression
           | simple_expression
           ;

var :  T_identifier
    |  T_identifier '[' expression ']'
    ;

simple_expression : additive_expression relop additive_expression
                  | additive_expression
                  ;

relop : T_lte
      | "<"
      | ">"
      | T_gte
      | T_eql
      | T_neq
      ;


additive_expression : additive_expression addop term
                    | term
                    ;

addop : '+'
      | '-'
      ;

term : term mulop factor
     | factor
     ;

mulop : '*'
      | '/'
      ;

factor : '(' expression ')'
       | var
       | call
       | T_intconst
       ;

call :  T_identifier '(' args ')'
     ;

args : arg_list
     | empty
     ;

arg_list : arg_list ',' expression
         | expression
         ;

empty : /* empty */
      ;
%%

int main() {
    return yyparse();
}