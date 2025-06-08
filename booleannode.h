#ifndef BOOLEANNODE_H
#define BOOLEANNODE_H

#include "logictypes.h"
#include "graphicnode.h"

#include <string>
#include <vector>
#include <functional>
#include <memory>

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

private:
    static int idCounter;
    std::string id;

    bool currentState;
    bool nextState;

    LogicFunction logicFunction;
    QList<GraphicNode*> inputNodes;
    QList<GraphicSwitch*> inputSwitches;
};

#endif // BOOLEANNODE_H
