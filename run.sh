bison -d -o cmmparser.cpp cmmparser.y
g++ -c `llvm-config --cppflags` -std=c++11 -o cmmparser.o cmmparser.cpp
flex -o cmmscanner.cpp cmmscanner.l
g++ -c `llvm-config --cppflags` -std=c++11 -o cmmscanner.o cmmscanner.cpp
g++ -c `llvm-config --cppflags` -std=c++11 -o main.o main.cpp
g++ -c `llvm-config --cppflags` -std=c++11 -o codegen.o codegen.cpp
g++ -c `llvm-config --cppflags` -std=c++11 -o corefn.o corefn.cpp
g++ -o test cmmparser.o cmmscanner.o main.o codegen.o corefn.o  `llvm-config --libs` `llvm-config --ldflags` -lpthread -ldl -lz -lncurses -rdynamic -no-pie