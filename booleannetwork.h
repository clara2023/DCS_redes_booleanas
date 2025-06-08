#ifndef BOOLEANNETWORK_H
#define BOOLEANNETWORK_H

#include "booleannode.h"

class BooleanNetwork
{
public:
    void addNode(BooleanNode* node);
    void simulateNetworkStep();
    void simulateNSteps(int nSteps);

private:
    std::vector<BooleanNode*> nodes;
};

#endif // BOOLEANNETWORK_H
