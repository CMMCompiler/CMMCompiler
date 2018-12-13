bison -vdty cmmparser.y
flex cmmscanner.l
gcc -o cmm.exe lex.yy.c y.tab.c