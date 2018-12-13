# CMMCompiler
编译原理课设

## Prepare

### MacOS

```
brew install flex
brew install bison
```
### Ubuntu

```
apt-get install flex bison
```

## Run

```
bison -vdty cmmparser.y # output y.tab.c y.tab.h y.output
flex cmmscanner.l # output: lex.yy.c
gcc -o cmmscanner lex.yy.c y.tab.c # output: cmmscanner
./cmmscanner < sample.c #output: out.txt
```
