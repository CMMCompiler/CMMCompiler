#include "node.h"
#include "codegen.h"
#include "cmmparser.hpp"

using namespace std;
#define ISTYPE(value, id) (value->getType()->getTypeID() == id)
static Type *typeOf(const NIdentifier& type, CodeGenContext& context);
Function* myMainFunction;
int mainFunctionNum = 0;

/* Compile the AST into a module */
void CodeGenContext::generateCode(NBlock& root)
{
	std::cout << "Generating code...\n";
	
	/* Create the top level interpreter function to call as entry */
	std::vector<Type*> argTypes;
	FunctionType *ftype = FunctionType::get(Type::getVoidTy(MyContext), makeArrayRef(argTypes), false);
	mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main");
	BasicBlock *bblock = BasicBlock::Create(MyContext, "entry");
	
	/* Push a new variable/block context */
	pushBlock(bblock);
	root.codeGen(*this); /* emit bytecode for the toplevel block */
	// ReturnInst::Create(MyContext, bblock);
	popBlock();
	
	/* Print the bytecode in a human-readable format 
	   to see if our program compiled properly
	 */
	std::cout << "Code is generated.\n";
	// module->dump();

	legacy::PassManager pm;
	pm.add(createPrintModulePass(outs()));
	pm.run(*module);
}

static Value* CastToBoolean(CodeGenContext& context, Value* condValue){

    if( ISTYPE(condValue, Type::IntegerTyID) ){
        condValue = context.builder.CreateIntCast(condValue, Type::getInt1Ty(context.MyContext), true);
        return context.builder.CreateICmpNE(condValue, ConstantInt::get(Type::getInt1Ty(context.MyContext), 0, true));
    }else if( ISTYPE(condValue, Type::DoubleTyID) ){
        return context.builder.CreateFCmpONE(condValue, ConstantFP::get(context.MyContext, APFloat(0.0)));
    }else{
		std::cout<< "no castToBoolean" <<std::endl;
        return condValue;
    }
}

/* Executes the AST by running the main function */
GenericValue CodeGenContext::runCode() {
	if (mainFunctionNum == 0){
		cout<< "ERROR: no main fun" <<endl;
		exit(0);
	}else 
	if (mainFunctionNum != 1){
		cout<< "ERROR: too many main fun" <<endl;
		exit(0);
	}
	std::cout << "Running code...\n";
	ExecutionEngine *ee = EngineBuilder( unique_ptr<Module>(module) ).create();
	ee->finalizeObject();
	vector<GenericValue> noargs;
	GenericValue v = ee->runFunction(myMainFunction, noargs);
	std::cout << "Code was run.\n";
	return v;
}

/* Returns an LLVM type based on the identifier */
static Type *typeOf(const NIdentifier& type, CodeGenContext& context) 
{
	if (type.name.compare("int") == 0) {
		std::cout<<"int"<<endl;
		return Type::getInt64Ty(context.MyContext);
	}
	return Type::getVoidTy(context.MyContext);
}

/* -- Code Generation -- */

Value* NInteger::codeGen(CodeGenContext& context)
{
	std::cout << "Creating integer: " << value << endl;
	return ConstantInt::get(Type::getInt64Ty(context.MyContext), value, true);
}

Value* NDouble::codeGen(CodeGenContext& context)
{
	std::cout << "Creating double: " << value << endl;
	return ConstantFP::get(Type::getDoubleTy(context.MyContext), value);
}

Value* NIdentifier::codeGen(CodeGenContext& context)
{
	std::cout << "Creating identifier reference: " << name << endl;
	if (context.locals().find(name) == context.locals().end()) {
		std::cerr << "undeclared variable " << name << endl;
		return NULL;
	}
	return new LoadInst(context.locals()[name], "", false, context.currentBlock());
}

Value* NBinaryOperator::codeGen(CodeGenContext& context)
{
	std::cout << "Creating binary operation " << op << endl;
	Instruction::BinaryOps instr;
	switch (op) {
		case T_add: 	instr = Instruction::Add; goto math;
		case T_minus: 	instr = Instruction::Sub; goto math;
		case T_times: 		instr = Instruction::Mul; goto math;
		case T_divide: 		instr = Instruction::SDiv; goto math;
				
		/* TODO comparison */
	}

	return NULL;
math:
	return BinaryOperator::Create(instr, lhs.codeGen(context), 
		rhs.codeGen(context), "", context.currentBlock());
}

Value* NAssignment::codeGen(CodeGenContext& context)
{
	std::cout << "Creating assignment for " << lhs.name << endl;
	if (context.locals().find(lhs.name) == context.locals().end()) {
		std::cerr << "undeclared variable " << lhs.name << endl;
		return NULL;
	}
	return new StoreInst(rhs.codeGen(context), context.locals()[lhs.name], false, context.currentBlock());
}

Value* NBlock::codeGen(CodeGenContext& context)
{
	StatementList::const_iterator it;
	Value *last = NULL;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cout << "Generating code for " << typeid(**it).name() << endl;
		last = (**it).codeGen(context);
	}
	std::cout << "Creating block" << endl;
	return last;
}

Value* NExpressionStatement::codeGen(CodeGenContext& context)
{
	std::cout << "Generating code for " << typeid(expression).name() << endl;
	return expression.codeGen(context);
}

Value* NReturnStatement::codeGen(CodeGenContext& context)
{
	std::cout << "Generating return code for " << typeid(result).name() << endl;
	Value *returnValue = result->codeGen(context);
	context.setCurrentReturnValue(returnValue);
	return returnValue;
}

Value* NVariableDeclaration::codeGen(CodeGenContext& context)
{
	std::cout << "Creating variable declaration " << type.name << " " << id.name << endl;
	AllocaInst *alloc = new AllocaInst(typeOf(type, context), id.name.c_str(), context.currentBlock());
	context.locals()[id.name] = alloc;
	return alloc;
}

Value* NFunctionDeclaration::codeGen(CodeGenContext& context)
{
	vector<Type*> argTypes;
	VariableList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		argTypes.push_back(typeOf((**it).type, context));
	}
	FunctionType *ftype = FunctionType::get(typeOf(type, context), makeArrayRef(argTypes), false);
	Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, id.name.c_str(), context.module);
	BasicBlock *bblock = BasicBlock::Create(context.MyContext, "entry", function, 0);

	if (id.name == "main"){
		myMainFunction = function;
		mainFunctionNum ++;
	}

	context.pushBlock(bblock);

	Function::arg_iterator argsValues = function->arg_begin();
    Value* argumentValue;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		(**it).codeGen(context);
		
		argumentValue = &*argsValues++;
		argumentValue->setName((*it)->id.name.c_str());
		StoreInst *inst = new StoreInst(argumentValue, context.locals()[(*it)->id.name], false, bblock);
	}
	
	block.codeGen(context);
	ReturnInst::Create(context.MyContext, context.getCurrentReturnValue(), bblock);

	context.popBlock();
	std::cout << "Creating function: " << id.name << endl;
	return function;
}

Value* NIfStatement::codeGen(CodeGenContext& context)
{
	cout << "Generating if statement" << endl;
	Value* condValue = condition.codeGen(context);
	if (!condValue ) {
		return nullptr;
	}
	condValue = CastToBoolean(context, condValue);
	Function* theFunction = context.currentBlock()->getParent();
	BasicBlock *ifTrue = BasicBlock::Create(context.MyContext, "then", theFunction, 0);
	BasicBlock *mergeBB = BasicBlock::Create(context.MyContext, "ifcont");
	Value *last = NULL;
	BranchInst::Create(ifTrue, mergeBB, condValue, context.currentBlock());
	context.pushBlock(ifTrue, context.locals());
	last = trueblock.codeGen(context);
	last = BranchInst::Create(mergeBB, context.currentBlock());
	context.popBlock();
	theFunction->getBasicBlockList().push_back(mergeBB);        //
    context.builder.SetInsertPoint(mergeBB);
	ReturnInst::Create(context.MyContext, mergeBB); //for test
	//context.pushBlock(mergeBB);
	return last;
}

Value* NIfElseStatement::codeGen(CodeGenContext& context)
{
	cout << "Generating ifelse statement" << endl;
	Value* condValue = condition.codeGen(context);
	if (!condValue ) {
		return nullptr;
	}
	condValue = CastToBoolean(context, condValue);
	Function* theFunction = context.currentBlock()->getParent();
	BasicBlock *ifTrue = BasicBlock::Create(context.MyContext, "then", theFunction, 0);
	BasicBlock *ifFalse = BasicBlock::Create(context.MyContext, "else");
	BasicBlock *mergeBB = BasicBlock::Create(context.MyContext, "ifcont");

	Value *last = NULL;
	BranchInst::Create(ifTrue, ifFalse, condValue, context.currentBlock());
	context.pushBlock(ifTrue, context.locals());
	last = trueblock.codeGen(context);
	last = BranchInst::Create(mergeBB, context.currentBlock());
	context.popBlock();

	theFunction->getBasicBlockList().push_back(ifFalse);
	context.builder.SetInsertPoint(ifFalse);
	context.pushBlock(ifFalse, context.locals());
	last = falseblock.codeGen(context);
	last = BranchInst::Create(mergeBB, context.currentBlock());
	context.popBlock();

	theFunction->getBasicBlockList().push_back(mergeBB);
    context.builder.SetInsertPoint(mergeBB);
	ReturnInst::Create(context.MyContext, mergeBB);
	//context.pushBlock(mergeBB);
	return last;	
}

Value* NIterationStatement::codeGen(CodeGenContext& context)
{
	cout << "Generating iteration statement" << endl;
	Function* theFunction = context.currentBlock()->getParent();
	BasicBlock *whileBlock = BasicBlock::Create(context.MyContext, "forloop", theFunction);
	BasicBlock *afterBlock = BasicBlock::Create(context.MyContext, "forcont");

	Value* condValue = condition.codeGen(context);
    if( !condValue )
        return nullptr;

	Value* last = NULL;
	condValue = CastToBoolean(context, condValue);
	BranchInst::Create(whileBlock, afterBlock, condValue, context.currentBlock());	
	context.pushBlock(whileBlock, context.locals());
	last = iterateblock.codeGen(context);
	last = BranchInst::Create(whileBlock, context.currentBlock());
	context.popBlock();

	condValue = condition.codeGen(context);
    condValue = CastToBoolean(context, condValue);
	BranchInst::Create(whileBlock, afterBlock, condValue, context.currentBlock());	

	theFunction->getBasicBlockList().push_back(afterBlock);
	context.builder.SetInsertPoint(afterBlock);
	return last;
}

Value* NComparisonExpression::codeGen(CodeGenContext& context)
{
	std::cout << "Creating comparison operation " << op << endl;
	CmpInst::Predicate com;
	switch (op) {
		case T_eql:		com = CmpInst::ICMP_EQ; goto compare;
		case T_neq:		com = CmpInst::ICMP_NE; goto compare;
		case T_lt:		com = CmpInst::ICMP_SLT;goto compare;
		case T_lte:		com = CmpInst::ICMP_SLE;goto compare;
		case T_gt:		com = CmpInst::ICMP_SGT;goto compare;
		case T_gte:		com = CmpInst::ICMP_SGE;goto compare;
				
		/* TODO comparison */
	}

	return NULL;
compare:
	return CmpInst::Create(*(new Instruction::OtherOps), com, lhs.codeGen(context), 
		rhs.codeGen(context), "", context.currentBlock());
}

Value* NCompoundStatementDeclaration::codeGen(CodeGenContext& context)
{
	StatementList::const_iterator it;
	ExpressionList::const_iterator it2;
	Value *last = NULL;
	for (it = statelist.begin(); it != statelist.end(); it++) {
		// std::cout << "Generating code for " << typeid(**it).name() << endl;
		last = (**it).codeGen(context);
	}
	for (it2 = expressionlist.begin(); it2 != expressionlist.end(); it2++) {
		last = (**it2).codeGen(context);
	}
	std::cout << "Creating Compound" << endl;
	return last;
}

Value* NCallNode::codeGen(CodeGenContext& context)
{
	Function *function = context.module->getFunction(id.name.c_str());
	if (function == NULL) {
		std::cerr << "no such function " << id.name << endl;
	}
	std::vector<Value*> args;
	ExpressionList::const_iterator it;
	for (it = arglist.begin(); it != arglist.end(); it++) {
		args.push_back((**it).codeGen(context));
	}
	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	std::cout << "Creating method call: " << id.name << endl;
	return call;
}

Value* NExpression::codeGen(CodeGenContext& context)
{

}

Value* NStatement::codeGen(CodeGenContext& context)
{
    
}