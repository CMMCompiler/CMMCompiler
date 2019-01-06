#include <iostream>
#include "node.h"
#include "codegen.h"

using namespace std;

extern int yyparse();
extern NBlock* programBlock;


int main() {
    // if (!yyparse()) {
    //     programBlock->print(0);
    // }
	yyparse();
    InitializeNativeTarget();
	InitializeNativeTargetAsmPrinter();
	InitializeNativeTargetAsmParser();
	CodeGenContext context;
	context.generateCode(*programBlock);
	// context.runCode();
}