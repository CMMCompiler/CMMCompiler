#include <iostream>
#include <vector>
// #include <llvm/Value.h>

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
    // virtual llvm::Value* codeGen(CodeGenContext& context) { }
};

class NExpression : public Node {
};

class NStatement : public Node {
};

class NInteger : public NExpression {
public:
    long long value;
    NInteger(long long value) : value(value) { }
    // virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NDouble : public NExpression {
public:
    double value;
    NDouble(double value) : value(value) { }
    // virtual llvm::Value* codeGen(CodeGenContext& context);
};

// used
class NIdentifier : public NExpression {
public:
    std::string name;
    NExpression expression;
    NIdentifier(std::string& name) : name(name) { }
    NIdentifier(std::string& name, NExpression& expression) :
        name(name), expression(expression) { }
    // virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NMethodCall : public NExpression {
public:
    const NIdentifier& id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier& id, ExpressionList& arguments) :
        id(id), arguments(arguments) { }
    NMethodCall(const NIdentifier& id) : id(id) { }
    // virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBinaryOperator : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NBinaryOperator(NExpression& lhs, NExpression& rhs, int op) :
        lhs(lhs), rhs(rhs), op(op) { }
    // virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NComparisonExpression : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NComparisonExpression(NExpression& lhs, NExpression& rhs, int op) :
        lhs(lhs), rhs(rhs), op(op) { }
};

class NAssignment : public NExpression {
public:
    NIdentifier& lhs;
    NExpression& rhs;
    NAssignment(NIdentifier& lhs, NExpression& rhs) : 
        lhs(lhs), rhs(rhs) { }
    // virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock() { }
    // virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NExpressionStatement : public NStatement {
public:
    NExpression& expression;
    NExpressionStatement(NExpression& expression) : 
        expression(expression) { }
    // virtual llvm::Value* codeGen(CodeGenContext& context);
};

// used
class NVariableDeclaration : public NStatement {
public:
    const NIdentifier& type;
    std::string id;
    std::string arrayRange;
    NVariableDeclaration(const NIdentifier& type, std::string& id) :
        type(type), id(id) { }
    NVariableDeclaration(const NIdentifier& type, std::string& id, std::string arrayRange) :
        type(type), id(id), arrayRange(arrayRange) { }
    // virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NCompoundStatementDeclaration: public NBlock {
public:
    StatementList statelist;
    ExpressionList expressionlist;
    NCompoundStatementDeclaration(StatementList statelist, ExpressionList expressionlist) :
        statelist(statelist), expressionlist(expressionlist) { }
};

class NReturnStatement: public NExpression {
public:
    NExpression result;
    NReturnStatement() { }
    NReturnStatement(NExpression& result) :
        result(result) { }
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
    // virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NIfStatement: public NExpression {
public:
    NExpression& condition;
    NExpression trueblock;
    NExpression falseblock;
    NIfStatement(NExpression& condition, NExpression& trueblock) :
        condition(condition), trueblock(trueblock) { }
    NIfStatement(NExpression& condition, NExpression& trueblock, NExpression& falseblock) :
        condition(condition), trueblock(trueblock), falseblock(falseblock) { }
};

class NIterationStatement: public NExpression {
public:
    NExpression& condition;
    NExpression iterateblock;
    NIterationStatement(NExpression& condition, NExpression& iterateblock) :
        condition(condition), iterateblock(iterateblock) { }
};

class NCallNode: public NExpression {
public:
    std::string id;
    ExpressionList arglist;
    NCallNode(std::string id, ExpressionList& arglist) :
        id(id), arglist(arglist) { }
};
