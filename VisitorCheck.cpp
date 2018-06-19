#include "VisitorCheck.h"
#include "Node.h"
#include <string>

void VisitorCheck::visitExpression(NExpression* n) {
    std::cout<<__func__<<std::endl;
    n->accept(*this);
}

void VisitorCheck::visitStatement(NStatement* n) {
    std::cout<<__func__<<std::endl;
    n->accept(*this);
}

void VisitorCheck::visitInteger(NInteger* n) {
    std::cout<<__func__<<std::endl;
    std::cout<<n->value<<std::endl;
}

void VisitorCheck::visitDouble(NDouble* n) {
    std::cout<<__func__<<std::endl;
    std::cout<<n->value<<std::endl;
}

void VisitorCheck::visitMethodCall(NMethodCall* n) {
    std::cout<<__func__<<std::endl;
    const_cast<NIdentifier*>(&(n->id))->accept(*this);
}

void VisitorCheck::visitIdentifier(NIdentifier* n) {
    std::cout<<__func__<<std::endl;
    std::cout<<n->name<<std::endl;
}

void VisitorCheck::visitVariableDeclaration(NVariableDeclaration* n) {
    std::cout<<__func__<<std::endl;
    const_cast<NIdentifier*>(&(n->type))->accept(*this);
    n->id.accept(*this);
    n->assignmentExpr->accept(*this);
}

void VisitorCheck::visitBinaryOperator(NBinaryOperator* n) {
    std::cout<<__func__<<std::endl;
    n->lhs.accept(*this);
    std::cout<<n->op<<std::endl;
    n->rhs.accept(*this);
    std::cout<<" "<<n->lhs.getNodeId()<<std::endl;
    std::cout<<" "<<n->rhs.getNodeId()<<std::endl;
    std::cout<<isa<NInteger>(n->lhs)<< " "<<isa<NInteger>(n->rhs)<<std::endl;
    if(isa<NInteger>(n->lhs) && isa<NInteger>(n->rhs)) {
        std::cout<<"Can reduce strength"<<std::endl;
        auto tmp = new NInteger(0);
        if(n->op == 274) {
            tmp->value = reinterpret_cast<NInteger*>(&(n->lhs))->value + reinterpret_cast<NInteger*>(&(n->rhs))->value;
            std::cout<<tmp->value<<std::endl;
            delete n;
            n = tmp;
        }
    }
}

void VisitorCheck::visitAssignment(NAssignment* n) {
    std::cout<<__func__<<std::endl;
    n->lhs.accept(*this);
    n->rhs.accept(*this);
}

void VisitorCheck::visitBlock(NBlock* n) {
    std::cout<<__func__<<std::endl;
    for(auto iter = n->statements.begin(); iter != n->statements.end(); iter++) {
        (*iter)->accept(*this);
    }
}

void VisitorCheck::visitExpressionStatement(NExpressionStatement* n) {
    std::cout<<__func__<<std::endl;
    n->expression.accept(*this);
}

void VisitorCheck::visitFunctionDeclaration(NFunctionDeclaration* n) {
    std::cout<<__func__<<std::endl;
    const_cast<NIdentifier*>(&(n->type))->accept(*this);
    const_cast<NIdentifier*>(&(n->id))->accept(*this);
    for(auto iter = n->arguments.begin(); iter != n->arguments.end(); iter++) {
        (*iter)->accept(*this);
    }
    n->block.accept(*this);
}