#include "booleannode.h"
#include "logictypes.h"

#include <iostream>

int BooleanNode::idCounter = 0;

BooleanNode::BooleanNode()
    : id("N" + std::to_string(idCounter++)),
      currentState(false),
      nextState(false) {}

void BooleanNode::computeNextState()
{
    QVector<bool> inputValues;

    for (GraphicNode* node : inputNodes) {
        inputValues.append(node->state);
    }

    for (GraphicSwitch* sw : inputSwitches) {
        inputValues.append(sw->state);
    }

    switch (logicFunction) {
        case LogicFunction::AND:
            nextState = std::all_of(inputValues.begin(), inputValues.end(), [](bool v) { return v; });
            break;

        case LogicFunction::OR:
            nextState = std::any_of(inputValues.begin(), inputValues.end(), [](bool v) { return v; });
            break;

        case LogicFunction::NOT:
            // NOT espera apenas uma entrada
            nextState = !inputValues.isEmpty() ? !inputValues[0] : false;
            break;

        case LogicFunction::NAND:
            nextState = !std::all_of(inputValues.begin(), inputValues.end(), [](bool v) { return v; });
            break;

        case LogicFunction::NOR:
            nextState = !std::any_of(inputValues.begin(), inputValues.end(), [](bool v) { return v; });
            break;

        case LogicFunction::XOR:
            {
                int count = std::count(inputValues.begin(), inputValues.end(), true);
                nextState = (count % 2 == 1);
            }
            break;

        default:
            nextState = false;
            break;
    }
}

bool BooleanNode::getCurrentState() const
{
    return currentState;
}

std::string BooleanNode::getId() const
{
    return id;
}

void BooleanNode::updateState()
{
    currentState = nextState;
}

void BooleanNode::addInputNode(GraphicNode *node)
{

}

void BooleanNode::addInputSwitch(GraphicSwitch* sw)
{

}

void BooleanNode::removeInputNode(GraphicNode *node)
{

}

void BooleanNode::removeInputSwitch(GraphicSwitch* sw)
{

}



