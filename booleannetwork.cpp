#include "booleannetwork.h"
#include "booleannode.h"

#include <vector>
#include <string>
#include <iostream>

void BooleanNetwork::addNode(const BooleanNode &node)
{
    nodes.push_back(node);
}

bool BooleanNetwork::connectNodes(const std::string &originId, const std::string &destinationId)
{
    int originIndex = getIndexPerId(originId);
    int destIndex = getIndexPerId(destinationId);

    if (originIndex == -1 || destIndex == -1) {
        std::cerr << "Erro ao conectar " << originId << " -> " << destinationId << std::endl;
        return false;
    }

    nodes[destIndex].addEntry(originIndex);
    return true;
}

void BooleanNetwork::simulateStep()
{
    for (auto& node : nodes) {
        node.calculateNextState(nodes);
    }
    for (auto& node : nodes) {
        node.updateState();
    }
}

void BooleanNetwork::simulate(int nSteps)
{
    for (int i = 0; i < nSteps; i++) {
        std::cout << "Step " << i + 1 << ":" << std::endl;
        printStates();
        simulateStep();
    }
    std::cout << "Final step:" << std::endl;
    printStates();
}

void BooleanNetwork::printStates() const
{
    for (const auto& node : nodes) {
        std::cout << node.getId() << ": " << node.getCurrentState() << "  ";
    }
    std::cout << std::endl;
}

int BooleanNetwork::getIndexPerId(const std::string &id) const
{
    for(size_t i = 0; i <nodes.size(); i++) {
        if (nodes[i].getId() == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

const std::vector<BooleanNode>& BooleanNetwork::getNodes() const
{
    return nodes;
}

