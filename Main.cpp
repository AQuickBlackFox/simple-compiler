#include <iostream>
#include "Node.h"
#include "VisitorCheck.h"

extern NBlock* programBlock;
extern int yyparse();

int main(int argc, char *argv[]) {
    yyparse();
    std::cout<<programBlock<<std::endl;
    std::cout<<isa<NIdentifier>(*programBlock)<<std::endl;
    std::cout<<isa<NBlock>(*programBlock)<<std::endl;
    std::cout<<programBlock->getNodeId()<<std::endl;
    VisitorCheck v;
    programBlock->accept(v);
    return 0;
}

