#include "booleannetwork.h"
#include "booleannode.h"

#include <vector>
#include <string>
#include <iostream>
#include <QDebug>

void BooleanNetwork::addNode(BooleanNode *node)
{
    nodes.push_back(node);
}

void BooleanNetwork::simulateNetworkStep()
{
    for (auto* node : nodes) {
        node->computeNextState();
    }
    for (auto* node : nodes) {
        node->updateState();
        qDebug() << "Valor do nodo: " << node->getCurrentState();
    }
}

void BooleanNetwork::simulateNSteps(int nSteps)
{
    for (int i = 0; i < nSteps; i++) {
        simulateNetworkStep();
    }
}



