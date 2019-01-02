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

* Download flexbison from:  [Flexbison](https://sourceforge.net/projects/winflexbison/)
* Add flexbison to system path


## Run

### MacOS & Ubuntu
```
bison -d -o cmmparser.cpp cmmparser.y
flex -o cmmscanner.cpp cmmscanner.l
gcc -o cmm cmmparser.cpp cmmscanner.cpp
./cmm < sample.c
```

### Windows
```
.\run.bat
.\cmm.exe < sample.c
```
