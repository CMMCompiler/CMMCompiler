%{
int cur_line_num = 1;
%}

DIGIT       ([0-9])
LETTER      ([a-zA-Z])
NUM         ({DIGIT}{DIGIT}*)
ID          ({LETTER}{LETTER}*)

%%
("/*"[\0-\127]*"*/") { printf("COMMENT\n"); }
"else" { printf("else\n"); }
"if" { printf("if\n"); }
"int" { printf("int\n"); }
"return" { printf("return\n"); }
"void" { printf("void\n"); }
"while" { printf("while\n"); }

"+" { printf("+\n"); }
"-" { printf("-\n"); }
"*" { printf("*\n"); }
"/" { printf("/\n"); }
"<" { printf("<\n"); }
"<=" { printf("<=\n"); }
">" { printf(">\n"); }
">=" { printf(">=\n"); }
"==" { printf("==\n"); }
"!=" { printf("!=\n"); }
"=" { printf("=\n"); }
";" { printf(";\n"); }
"," { printf(",\n"); }
"(" { printf("(\n"); }
")" { printf(")\n"); }
"[" { printf("[\n"); }
"]" { printf("]\n"); }
"{" { printf("{\n"); }
"}" { printf("}\n"); }


[\n] { cur_line_num++; }
[ \t\r\a]+ { /* ignore spaces */ }

{NUM} { printf("NUM\n"); }
{ID} { printf("ID\n"); }
%%

int main(int argc, char* agrv[]) {
    yylex();
}

int yywrap(void) {
    return 1;
}