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
### Windows

```
Download flexbison from : [Flexbison](https://sourceforge.net/projects/winflexbison/)
Add flexbison to system path
```

## Run

### MacOS & Ubuntu
```
bison -vdty cmmparser.y # output y.tab.c y.tab.h y.output
flex cmmscanner.l # output: lex.yy.c
gcc -o cmm lex.yy.c y.tab.c # output: cmmscanner
./cmm < sample.c
```

###Windows
```
.\run.bat
.\cmm.exe < sample.c
```
