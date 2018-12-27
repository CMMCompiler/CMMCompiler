bison -vdty cmmparser.y
flex cmmscanner.l
gcc -o cmm lex.yy.c y.tab.c