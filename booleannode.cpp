#include "booleannode.h"
#include "graphicnode.h"
#include "logictypes.h"

#include <iostream>
#include <QDebug>
#include <string>
#include <vector>
#include <functional>
#include <memory>

int BooleanNode::idCounter = 0;

BooleanNode::BooleanNode()
    : currentState(false),
      nextState(false),
      id("N" + std::to_string(idCounter++)) {}

void BooleanNode::computeNextState()
{
    QVector<bool> inputValues;
    qDebug() << "=== Estado dos inputs de" << QString::fromStdString(id) << "===";

    qDebug() << ">> GraphicNodes:";
    for (GraphicNode* node : inputNodes) {
        bool state = node->state;
        QString nodeId = node->getBooleanNode() ? QString::fromStdString(node->getBooleanNode()->getId()) : "sem ID";
        inputValues.append(state);
        qDebug() << "   Node ID:" << nodeId << "Estado:" << (state ? "1" : "0");
    }

    qDebug() << ">> GraphicSwitches:";
    for (GraphicSwitch* sw : inputSwitches) {
        bool state = sw->state;
        inputValues.append(state);
        qDebug() << "   Switch Estado:" << (state ? "1" : "0");
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

        case LogicFunction::EQUAL:
            if (!inputValues.isEmpty()) {
                nextState = inputValues.last();  //  herda o último valor
            } else {
                nextState = false;
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

void BooleanNode::setFunction(LogicFunction func)
{
    logicFunction = func;
}

void BooleanNode::addInputNode(GraphicNode* node) {
    if (!inputNodes.contains(node)) {
        inputNodes.append(node);
    }
}

void BooleanNode::addInputSwitch(GraphicSwitch* sw) {
    inputSwitches.append(sw);
}

