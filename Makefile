all: test

OBJS = cmmparser.o  \
       cmmscanner.o \
	   main.o \
	   codegen.o \
	   corefn.o \

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11 -I/usr/local/opt/llvm@4/include
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic -L/usr/local/opt/llvm@4/lib
LIBS = `$(LLVMCONFIG) --libs`

clean:
	$(RM) -rf cmmparser.cpp cmmparser.hpp test cmmscanner.cpp $(OBJS)

cmmparser.cpp: cmmparser.y
	bison -d -o $@ $^

cmmparser.hpp: cmmparser.cpp

cmmscanner.cpp: cmmscanner.l
	flex -o cmmscanner.cpp cmmscanner.l

%.o: %.cpp
	g++ -c $(CPPFLAGS) -o $@ $<

test: $(OBJS)
	g++ -o $@ $(OBJS) $(LIBS) $(LDFLAGS)