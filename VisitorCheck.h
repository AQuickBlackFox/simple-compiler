#pragma once

#include "Visitor.h"

class VisitorCheck : public Visitor {
public:
void visitExpression(NExpression* n);
void visitStatement(NStatement* n);
void visitInteger(NInteger* n);
void visitDouble(NDouble* n);
void visitIdentifier(NIdentifier* n);
void visitMethodCall(NMethodCall* n);
void visitVariableDeclaration(NVariableDeclaration* n);
void visitBinaryOperator(NBinaryOperator* n);
void visitAssignment(NAssignment* n);
void visitBlock(NBlock* n);
void visitExpressionStatement(NExpressionStatement* n);
void visitFunctionDeclaration(NFunctionDeclaration* n);
};
