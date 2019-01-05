#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>

class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;

class Node {
public:
    virtual ~Node() {}
    virtual void print(int depth)const{}
    void print_token(int token)const{
        switch(token){
            case 258:puts("T_intconst");break;
            case 259:puts("T_identifier");break;
            case 260:puts("T_void");break;
            case 261:puts("T_int");break;
            case 262:puts("T_lt");break;
            case 263:puts("T_gt");break;
            case 264:puts("T_lte");break;
            case 265:puts("T_gte");break;
            case 266:puts("T_eql");break;
            case 267:puts("T_neq");break;
            case 268:puts("T_while");break;
            case 269:puts("T_if");break;
            case 270:puts("T_else");break;
            case 271:puts("T_return");break;
            case 272:puts("T_add");break;
            case 273:puts("T_minus");break;
            case 274:puts("T_times");break;
            case 275:puts("T_divide");break;
            case 276:puts("T_assign");break;
            case 277:puts("T_lparen");break;
            case 278:puts("T_rparen");break;
            case 279:puts("T_lbracket");break;
            case 280:puts("T_rbracket");break;
            case 281:puts("T_lbrace");break;
            case 282:puts("T_rbrace");break;
            case 283:puts("T_semicolon");break;
            case 284:puts("T_comma");break;
            default: puts("unknown token_type");
        }
    }
    // virtual llvm::Value* codeGen(CodeGenContext& context) { }
};

class NExpression : public Node {
public:
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NStatement : public Node {
public:
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NInteger : public NExpression {
public:
    long long value;
    NInteger(long long value) : value(value) {}
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        printf("Int value: ");
        std::cout<<value<<std::endl;
    }
};

class NDouble : public NExpression {
public:
    double value;
    NDouble(double value) : value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        printf("Double value: ");
        std::cout<<value<<std::endl;
    }
};

// used
class NIdentifier : public NExpression {
public:
    std::string name;
    NExpression* expression;
    NIdentifier(std::string& name) : name(name) { }
    NIdentifier(std::string& name, NExpression* expression) :
        name(name), expression(expression) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        if(name=="void"||name=="int")std::cout<<"Type: "<<name<<std::endl;
        else{
            std::cout<<"Id: "<<name;
            if (expression!=NULL){
                std::cout<<"[]"<<std::endl;
                for(int i=0;i<depth;i++)
                    printf("    ");
                std::cout<<"ArrayId:"<<std::endl;
                expression->print(depth+1);
            }
            else std::cout<<std::endl;
        }
    }
};

// class NMethodCall : public NExpression {
// public:
//     const NIdentifier& id;
//     ExpressionList arguments;
//     NMethodCall(const NIdentifier& id, ExpressionList& arguments) :
//         id(id), arguments(arguments) { }
//     NMethodCall(const NIdentifier& id) : id(id) { }
//     virtual llvm::Value* codeGen(CodeGenContext& context);
//     void print(int depth)const{
//         for(int i=0;i<depth;i++)
//             printf("    ");
//         puts("Method Call");
//         id.print(depth+1);
//         for(auto it=arguments.begin();it!=arguments.end();it++){
//             (*it)->print(depth+1);
//         }
//     }
// };

class NBinaryOperator : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NBinaryOperator(NExpression& lhs, NExpression& rhs, int op) :
        lhs(lhs), rhs(rhs), op(op) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("Binary Operator");
        for(int i=0;i<=depth;i++)
            printf("    ");
        printf("Op: ");
        print_token(op);
        lhs.print(depth+1);
        rhs.print(depth+1);
    }
};

class NComparisonExpression : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NComparisonExpression(NExpression& lhs, NExpression& rhs, int op) :
        lhs(lhs), rhs(rhs), op(op) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("Comparison Expression");
        for(int i=0;i<=depth;i++)
            printf("    ");
        printf("Op: ");
        print_token(op);
        lhs.print(depth+1);
        rhs.print(depth+1);
    }
};

class NAssignment : public NExpression {
public:
    NIdentifier& lhs;
    NExpression& rhs;
    NAssignment(NIdentifier& lhs, NExpression& rhs) : 
        lhs(lhs), rhs(rhs) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("Assignment");
        lhs.print(depth+1);
        rhs.print(depth+1);
    }
    
};

class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock() { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(auto it=statements.begin();it!=statements.end();it++)
            (*it)->print(depth+1);
    }
};

class NExpressionStatement : public NStatement {
public:
    NExpression& expression;
    NExpressionStatement(NExpression& expression) : 
        expression(expression) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("Expression Statement");
        expression.print(depth+1);
    }

};

// used
class NVariableDeclaration : public NStatement {
public:
    const NIdentifier& type;
    NIdentifier& id;
    std::string arrayRange;
    NVariableDeclaration(const NIdentifier& type, NIdentifier& id) :
        type(type), id(id) { }
    NVariableDeclaration(const NIdentifier& type, NIdentifier& id, std::string arrayRange) :
        type(type), id(id), arrayRange(arrayRange) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("Variable Declaration");
        type.print(depth+1);for(int i=0;i<=depth;i++)
            printf("    ");
        std::cout<<"Id: "<<id.name<<std::endl;
        if(arrayRange.length()){
            for(int i=0;i<=depth;i++)
                printf("    ");
            std::cout<<"ArrayRange: "<<arrayRange<<std::endl;
        }
    }
};

class NCompoundStatementDeclaration: public NBlock {
public:
    StatementList statelist;
    ExpressionList expressionlist;
    NCompoundStatementDeclaration(StatementList& statelist, ExpressionList& expressionlist) :
        statelist(statelist), expressionlist(expressionlist) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("Compound Statement Declaration");
        for(auto it=statelist.begin();it!=statelist.end();it++){
            (*it)->print(depth+1);
        }
        for(auto it=expressionlist.begin();it!=expressionlist.end();it++){
            (*it)->print(depth+1);
        }
    }
};

class NReturnStatement: public NExpression {
public:
    NExpression* result;
    NReturnStatement() { }
    NReturnStatement(NExpression* result) :
        result(result) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("Return");
        if(result!=NULL)result->print(depth+1);
    }
};

// used
class NFunctionDeclaration : public NStatement {
public:
    const NIdentifier& type;
    std::string id;
    VariableList arguments;
    NBlock& block;
    NFunctionDeclaration(const NIdentifier& type, std::string& id, 
            const VariableList& arguments, NBlock& block) :
        type(type), id(id), arguments(arguments), block(block) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("Function Declaration");
        type.print(depth+1);
        for(int i=0;i<=depth;i++)
            printf("    ");
        std::cout<<"Id: "<<id<<std::endl;
        for(auto it=arguments.begin();it!=arguments.end();it++){
            (*it)->print(depth+1);
        }
        block.print(depth+1);
    }

};

class NIfStatement: public NExpression {
public:
    NExpression& condition;
    NExpression& trueblock;
    NIfStatement(NExpression& condition, NExpression& trueblock) :
        condition(condition), trueblock(trueblock) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("If");
        condition.print(depth+1);
        trueblock.print(depth+1);
    }
};

class NIfElseStatement: public NExpression {
public:
    NExpression& condition;
    NExpression& trueblock;
    NExpression& falseblock;
    NIfElseStatement(NExpression& condition, NExpression& trueblock, NExpression& falseblock) :
        condition(condition), trueblock(trueblock), falseblock(falseblock) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("If");
        condition.print(depth+1);
        trueblock.print(depth+1);
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("Else");
        falseblock.print(depth+1);
    }
};

class NIterationStatement: public NExpression {
public:
    NExpression& condition;
    NExpression& iterateblock;
    NIterationStatement(NExpression& condition, NExpression& iterateblock) :
        condition(condition), iterateblock(iterateblock) { }   
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("Iteration Statement");
        condition.print(depth+1);
        iterateblock.print(depth+1);
    }
};

class NCallNode: public NExpression {
public:
    std::string id;
    ExpressionList& arglist;
    NCallNode(std::string id, ExpressionList& arglist) :
        id(id), arglist(arglist) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
    void print(int depth)const{
        for(int i=0;i<depth;i++)
            printf("    ");
        puts("Call");
        for (int i=0;i<depth+1;i++) {
            printf("    ");
        }
        std::cout<<"function_name: "<<id<<std::endl;
        for(auto it=arglist.begin();it!=arglist.end();it++) {
            (*it)->print(depth+1);
        }
    }
};
