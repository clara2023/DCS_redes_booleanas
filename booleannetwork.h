#ifndef BOOLEANNETWORK_H
#define BOOLEANNETWORK_H

#include "booleannode.h"


class BooleanNetwork
{
public:
    void addNode(const BooleanNode& node);
    bool connectNodes(const std::string& originId, const std::string& destinationId);

    void simulateStep();
    void simulate(int nSteps);

    void printStates() const;
    const std::vector<BooleanNode>& getNodes() const;

private:
    std::vector<BooleanNode> nodes;
    int getIndexPerId(const std::string& id) const;
};

#endif // BOOLEANNETWORK_H
