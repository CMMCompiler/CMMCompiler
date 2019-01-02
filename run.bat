bison -d -o cmmparser.cpp cmmparser.y
flex -o tokens.cpp cmmscanner.l
g++ -o cmm.exe cmmparser.cpp tokens.cpp