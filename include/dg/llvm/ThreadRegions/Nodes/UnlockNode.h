#ifndef UNLOCKNODE_H
#define UNLOCKNODE_H

#include "Node.h"

class UnlockNode : public Node
{
public:
    UnlockNode(const llvm::Instruction * instruction = nullptr, const llvm::CallInst * callInst = nullptr);
};

#endif // UNLOCKNODE_H
