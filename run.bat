bison -d -o parser.cpp cmmparser.y
flex -o tokens.cpp cmmscanner.l
g++ -o cmm.exe parser.cpp tokens.cpp