#include "dg/llvm/ThreadRegions/Nodes/CallFuncPtrNode.h"

CallFuncPtrNode::CallFuncPtrNode(const llvm::Instruction *instruction)
    :Node(NodeType::CALL_FUNCPTR, instruction)
{}
