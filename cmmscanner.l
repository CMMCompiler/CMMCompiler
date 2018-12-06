%{
#include "token.h"
int cur_line_num = 1;
%}

DIGIT       ([0-9])
LETTER      ([a-zA-Z])
NUM         ({DIGIT}{DIGIT}*)
ID          ({LETTER}{LETTER}*)
OPERATOR    ([+-/*<>=;,(){}])

%%
{OPERATOR} { return yytext[0]; }
("/*"[\0-\127]*"*/") { return T_comment; }
"else" { return T_else; }
"if" { return T_if; }
"int" { return T_int; }
"return" { return T_return; }
"void" { return T_void; }
"while" { return T_while; }

"<=" { return T_lte; }
">=" { return T_gte; }
"==" { return T_eql; }
"!=" { return T_neq; }

[\n] { cur_line_num++; }
[ \t\r\a]+ { /* ignore spaces */ }

{NUM} { return T_intconst; }
{ID} { return T_identifier; }
%%

int main(int argc, char* agrv[]) {
    int token;
    while (token = yylex()) {
        print_token(token);
        puts(yytext);
    }
    return 0;
}

int yywrap(void) {
    return 1;
}