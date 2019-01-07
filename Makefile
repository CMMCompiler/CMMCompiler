all: test

OBJS = cmmparser.o  \
       codegen.o \
       main.o    \
       cmmscanner.o  \
       corefn.o  \
	   native.o \

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags` -std=c++11
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic -no-pie
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


cmm: $(OBJS)
	g++ -o $@ $(OBJS) $(LIBS) $(LDFLAGS)

test: cmm sample.c
	cat sample.c | ./cmm