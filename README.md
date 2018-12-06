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
flex cmmscanner.l # output: lex.yy.c
gcc -o cmmscanner lex.yy.c # output: cmmscanner
./cmmscanner < sample.c #output: out.txt
```
