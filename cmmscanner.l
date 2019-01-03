%{

#include <stdio.h>
#include <string>
#include "node.h"
#include "cmmparser.hpp"
using namespace std;
#define SAVE_TOKEN yylval.string = new string(yytext)
#define TOKEN(t) ( yylval.token = t)
int cur_line_num = 1;
%}

DIGIT       ([0-9])
LETTER      ([a-zA-Z])
NUM         ({DIGIT}{DIGIT}*)
ID          ({LETTER}{LETTER}*)
OPERATOR    ([+-/*<>=;,(){}])

%option noyywrap

%%
[ \t\r\a\n] { }
("/*"(([^\*]*(\*[^\/])?)*)"*/") { }
"else" { puts("T_else");return TOKEN(T_else); }
"if" { puts("T_if");return TOKEN(T_if); }
"int" { SAVE_TOKEN;puts("T_int");return T_int; }
"return" { puts("T_return");return TOKEN(T_return); }
"void" { SAVE_TOKEN;puts("T_void");return T_void; }
"while" { puts("T_while");return TOKEN(T_while); }

"<"  { puts("T_lt");return TOKEN(T_lt); }
">"  { puts("T_gt");return TOKEN(T_gt); }
"<=" { puts("T_lte");return TOKEN(T_lte); }
">=" { puts("T_gte");return TOKEN(T_gte); }
"==" { puts("T_eql");return TOKEN(T_eql); }
"!=" { puts("T_neq");return TOKEN(T_neq); }

"+" { puts("T_add");return TOKEN(T_add);}
"-" { puts("T_minus");return TOKEN(T_minus); }
"*" { puts("T_times");return TOKEN(T_times); }
"/" { puts("T_divide");return TOKEN(T_divide); }
"(" { puts("T_lparen");return TOKEN(T_lparen); }
")" { puts("T_rparen");return TOKEN(T_rparen); }
";" { puts("T_semicolon");return TOKEN(T_semicolon); }
"," { puts("T_comma");return TOKEN(T_comma); }
"=" { puts("T_assign");return TOKEN(T_assign); }
"[" { puts("T_lbracket");return TOKEN(T_lbracket); }
"]" { puts("T_rbracket");return TOKEN(T_rbracket); }
"{" { puts("T_lbrace");return TOKEN(T_lbrace); }
"}" { puts("T_rbrace");return TOKEN(T_rbrace); }
[a-zA-Z_][a-zA-Z0-9_]*	SAVE_TOKEN; puts("T_identifier"); return T_identifier;
[0-9]+  				SAVE_TOKEN; puts("T_intconst"); return T_intconst;
. printf("Unknown token:%s\n", yytext); yyterminate();
%%