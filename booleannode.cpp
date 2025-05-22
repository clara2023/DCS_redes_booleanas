#include "booleannode.h"

#include <iostream>

BooleanNode::BooleanNode(const std::string& id) : id(id), currentState(false), nextState(false) {}

void BooleanNode::addEntry(size_t nodeId)
{
    entriesIndex.push_back(nodeId);
}

void BooleanNode::defineLogicFunction(const std::function<bool (const std::vector<bool> &)> &newFunction)
{
    logicFunction = newFunction;
}

void BooleanNode::calculateNextState(std::vector<BooleanNode> &allNodes)
{
    if(!logicFunction)
    {
        std::cerr << "Função lógica não definida para nodo " << id << std::endl;
    }

    std::vector<bool> entriesStates;
    for(auto index : entriesStates) {
        entriesStates.push_back(allNodes[index].getCurrentState());
    }

    nextState = logicFunction(entriesStates);
}

void BooleanNode::updateState()
{
    currentState = nextState;
}

bool BooleanNode::getCurrentState() const
{
    return currentState;
}

std::string BooleanNode::getId() const
{
    return id;
}





