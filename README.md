# CMMCompiler
编译原理课设

## Prepare

### MacOS

```
brew install flex
brew install bison
```

## Run

```
flex cmmscanner.l # output: let.yy.c
gcc -o cmmscanner lex.yy.c # output: cmmscanner
./scanner < sample.c > out.txt #output: out.txt
```
