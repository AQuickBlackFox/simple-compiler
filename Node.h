#pragma once

#include <iostream>
#include <vector>
#include "Visitor.h"

template<typename To, typename From>
bool isa(From const& f) {
    return To::classof(&f);
}

class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;

enum NodeId_t {
    NodeId = 0,
    ExprId,
    StmtId,
    IntId,
    DoubleId,
    IdentId,
    MCallId,
    BinOpId,
    AssignId,
    BlockId,
    VarDecId,
    FuncDecId,
    ExprStmtId,
};

class Node {
public:
    virtual ~Node() {}
    static inline bool classof(Node const*) { return true; }
    virtual NodeId_t getNodeId() const = 0;
    virtual void accept(Visitor& v) = 0;
};

class NExpression : public Node {
public:
    virtual ~NExpression() {}
    static inline bool classof(NExpression const*) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == ExprId; }
    virtual NodeId_t getNodeId() { return ExprId; }
    virtual void accept(Visitor& v) { v.visitExpression(this); }
};

class NStatement : public Node {
public:
    virtual ~NStatement() {}
    static inline bool classof(NStatement const*) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == StmtId; }
    virtual NodeId_t getNodeId() const { return StmtId; }
    virtual void accept(Visitor& v) { v.visitStatement(this); }
};

class NInteger : public NExpression {
public:
    long long value;
    NInteger(long long value) : value(value) {}
    virtual ~NInteger() {}
    static inline bool classof(NInteger const*) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == IntId; }
    virtual NodeId_t getNodeId() const { return IntId; }
    virtual void accept(Visitor& v) { v.visitInteger(this); }
};

class NDouble : public NExpression {
public:
    double value;
    NDouble(double value) : value(value) {}
    virtual ~NDouble() {}
    static inline bool classof(NDouble const*) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == DoubleId; }
    virtual NodeId_t getNodeId() const { return DoubleId; }
    virtual void accept(Visitor& v) { v.visitDouble(this); }
};

class NIdentifier : public NExpression {
public:
    std::string name;
    NIdentifier(const std::string &name) : name(name) {}
    virtual ~NIdentifier() {}
    static inline bool classof(NIdentifier const*) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == IdentId; }
    virtual NodeId_t getNodeId() const { return IdentId; }
    virtual void accept(Visitor& v) { v.visitIdentifier(this); }
};

class NMethodCall : public NExpression {
public:
    const NIdentifier& id;
    ExpressionList arguments;
    NMethodCall(const NIdentifier& id, ExpressionList& arguments) :
        id(id), arguments(arguments) {}
    NMethodCall(const NIdentifier &id) : id(id) {}
    virtual ~NMethodCall() {}
    static inline bool classof(NMethodCall const *) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == MCallId; }
    virtual NodeId_t getNodeId() const { return MCallId; }
    virtual void accept(Visitor& v) { v.visitMethodCall(this); }
};

class NBinaryOperator : public NExpression {
public:
    int op;
    NExpression& lhs;
    NExpression& rhs;
    NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) :
        lhs(lhs), op(op), rhs(rhs) {}
    virtual ~NBinaryOperator() {}
    static inline bool classof(NBinaryOperator const *) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == BinOpId; }
    virtual NodeId_t getNodeId() const { return BinOpId; }
    virtual void accept(Visitor& v) { v.visitBinaryOperator(this); }
};

class NAssignment : public NExpression {
public:
    NIdentifier& lhs;
    NExpression& rhs;
    NAssignment(NIdentifier& lhs, NExpression& rhs) :
        lhs(lhs), rhs(rhs) {}
    virtual ~NAssignment() {}
    static inline bool classof(NAssignment const *) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == AssignId; }
    inline NodeId_t getNodeId() const { return AssignId; }
    virtual void accept(Visitor& v) { v.visitAssignment(this); }
};

class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock() {}
    virtual ~NBlock() {}
    static inline bool classof(NBlock const *) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == BlockId; }
    virtual NodeId_t getNodeId() const { return BlockId; }
    virtual void accept(Visitor& v) { v.visitBlock(this); }
};

class NExpressionStatement : public NStatement {
public:
    NExpression& expression;
    NExpressionStatement(NExpression& expression) :
        expression(expression) {}
    virtual ~NExpressionStatement() {}
    static inline bool classof(NExpressionStatement const *) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == ExprStmtId; }
    virtual NodeId_t getNodeId() const { return ExprStmtId; }
    virtual void accept(Visitor& v) { v.visitExpressionStatement(this); }
};

class NVariableDeclaration : public NStatement {
public:
    const NIdentifier& type;
    NIdentifier& id;
    NExpression *assignmentExpr;
    NVariableDeclaration(const NIdentifier& type, NIdentifier& id) :
        type(type), id(id) {}
    NVariableDeclaration(const NIdentifier& type, NIdentifier& id, NExpression *assignmentExpr) :
        type(type), id(id), assignmentExpr(assignmentExpr) {}
    virtual ~NVariableDeclaration() {}
    static inline bool classof(NVariableDeclaration const* ) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == VarDecId; }
    virtual NodeId_t getNodeId() const { return VarDecId; }
    virtual void accept(Visitor& v) { v.visitVariableDeclaration(this); }
};

class NFunctionDeclaration : public NStatement {
public:
    const NIdentifier& type;
    const NIdentifier& id;
    VariableList arguments;
    NBlock& block;
    NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id, const VariableList& arguments, NBlock& block) :
        type(type), id(id), arguments(arguments), block(block) {}
    virtual ~NFunctionDeclaration() {}
    static inline bool classof(NFunctionDeclaration const *) { return true; }
    static inline bool classof(Node const *B) { return B->getNodeId() == FuncDecId; }
    virtual NodeId_t getNodeId() const { return FuncDecId; }
    virtual void accept(Visitor& v) { v.visitFunctionDeclaration(this); }
};