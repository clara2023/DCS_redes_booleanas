#ifndef BOOLEANNODE_H
#define BOOLEANNODE_H

#include <string>
#include <vector>
#include <functional>
#include <memory>

class BooleanNode
{
public:
    BooleanNode(const std::string& id);

    void addEntry(size_t nodeId);
    void defineLogicFunction(const std::function<bool(const std::vector<bool>&)>& newFunction);

    void calculateNextState(std::vector<BooleanNode>& allNodes);
    void updateState();

    bool getCurrentState() const;
    std::string getId() const;

private:
    std::string id;
    bool currentState;
    bool nextState;
    std::vector<size_t> entriesIndex;
    std::function<bool(const std::vector<bool>&)> logicFunction;

};

#endif // BOOLEANNODE_H
