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
"else" { return TOKEN(T_else); }
"if" { return TOKEN(T_if); }
"int" { SAVE_TOKEN;return T_int; }
"return" { return TOKEN(T_return); }
"void" { SAVE_TOKEN;return T_void; }
"while" { return TOKEN(T_while); }

"<"  { return TOKEN(T_lt); }
">"  { return TOKEN(T_gt); }
"<=" { return TOKEN(T_lte); }
">=" { return TOKEN(T_gte); }
"==" { return TOKEN(T_eql); }
"!=" { return TOKEN(T_neq); }

"+" { return TOKEN(T_add);}
"-" { return TOKEN(T_minus); }
"*" { return TOKEN(T_times); }
"/" { return TOKEN(T_divide); }
"(" { return TOKEN(T_lparen); }
")" { return TOKEN(T_rparen); }
";" { return TOKEN(T_semicolon); }
"," { return TOKEN(T_comma); }
"=" { return TOKEN(T_assign); }
"[" { return TOKEN(T_lbracket); }
"]" { return TOKEN(T_rbracket); }
"{" { return TOKEN(T_lbrace); }
"}" { return TOKEN(T_rbrace); }
[a-zA-Z_][a-zA-Z0-9_]*	SAVE_TOKEN; return T_identifier;
[0-9]+  				SAVE_TOKEN; return T_intconst;
. printf("Unknown token:%s\n", yytext); yyterminate();
%%