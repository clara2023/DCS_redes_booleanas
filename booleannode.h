#ifndef BOOLEANNODE_H
#define BOOLEANNODE_H

#include "logictypes.h"

#include <string>
#include <vector>
#include <functional>
#include <memory>

class GraphicNode;
class GraphicSwitch;

class BooleanNode
{
public:
    BooleanNode();

    void addInputNode(GraphicNode* node);
    void addInputSwitch(GraphicSwitch* sw);

    void removeInputNode(GraphicNode* node);
    void removeInputSwitch(GraphicSwitch* sw);

    void setFunction(LogicFunction func);
    void computeNextState();
    void updateState();

    bool getCurrentState() const;
    std::string getId() const;

    LogicFunction logicFunction;
    QList<GraphicNode*> inputNodes;
    QList<GraphicSwitch*> inputSwitches;

    bool currentState;
    bool nextState;

private:
    static int idCounter;
    std::string id;
};

#endif // BOOLEANNODE_H
