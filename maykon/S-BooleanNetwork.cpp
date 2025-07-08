#include "BooleanNetwork.h"
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/Parser_if.h"
#include "../../kernel/simulator/PluginManager.h"

#ifdef PLUGINCONNECT_DYNAMIC
extern "C" StaticGetPluginInformation GetPluginInformation() {
    return &BooleanNetwork::GetPluginInformation;
}
#endif

BooleanNetwork::BooleanNetwork(Model* model, std::string name)
    : ModelComponent(model, Util::TypeOf<BooleanNetwork>(), name) {}

void BooleanNetwork::addNode(BooleanNetworkNode* node) {
    _nodes.push_back(node);
}

std::vector<BooleanNetworkNode*> BooleanNetwork::getNodes() const {
    return _nodes;
}

PluginInformation* BooleanNetwork::GetPluginInformation() {
    PluginInformation* info = new PluginInformation(Util::TypeOf<BooleanNetwork>(), &BooleanNetwork::LoadInstance, &BooleanNetwork::NewInstance);
    return info;
}

std::string BooleanNetwork::show() {
    std::string txt = ModelComponent::show() + ", nodes=[";

    for (auto node : _nodes) {
        txt += node->getName() + ",";
    }

    if (!_nodes.empty())
        txt = txt.substr(0, txt.length() - 1);

    txt += "]";
    return txt;
}

ModelComponent* BooleanNetwork::LoadInstance(Model* model, PersistenceRecord *fields) {
    BooleanNetwork* newComponent = new BooleanNetwork(model);
    newComponent->_loadInstance(fields);
    return newComponent;
}

ModelDataDefinition* BooleanNetwork::NewInstance(Model* model, std::string name) {
    return new BooleanNetwork(model, name);
}

bool BooleanNetwork::_loadInstance(PersistenceRecord *fields) {
    bool res = ModelComponent::_loadInstance(fields);
    if (res) {
        unsigned int numNodes = fields->loadField("nodesCount", 0);
        for (unsigned int i = 0; i < numNodes; i++) {
            std::string nodeName = fields->loadField("node_" + std::to_string(i), "");
            ModelDataDefinition* mdd = _parentModel->getDataManager()->getDataDefinition(Util::TypeOf<BooleanNetworkNode>(), nodeName);

            if (mdd != nullptr) {
                BooleanNetworkNode* node = dynamic_cast<BooleanNetworkNode*>(mdd);
                this->addNode(node);
            }
        }
    }
    return res;
}

void BooleanNetwork::_saveInstance(PersistenceRecord *fields, bool saveDefaultValues) {
    ModelComponent::_saveInstance(fields, saveDefaultValues);

    fields->saveField("nodesCount", _nodes.size(), 0u, saveDefaultValues);

    unsigned int i = 0;
    for (BooleanNetworkNode* node : _nodes) {
        fields->saveField("node_" + std::to_string(i), node->getName(), "", saveDefaultValues);
        i++;
    }
}

void BooleanNetwork::_onDispatchEvent(Entity* entity, unsigned int inputPortNumber) {
    for (auto node : _nodes) {
        std::string expr = node->getExpression();
        for (const std::string& input : node->getInputs()) {
            double value = entity->getAttributeValue(input);
            expr.replace(expr.find(input), input.length(), std::to_string(value));
        }

        

        double result = _parentModel->parseExpression(expr);
        bool boolResult = result != 0.0;
        node->setValue(boolResult);

        std::string dest = node->getName();
        entity->setAttributeValue(dest, boolResult ? 1.0 : 0.0);

        traceSimulation(this, "Node \"" + dest + "\" = " + std::to_string(boolResult ? 1 : 0) + "  // " + node->getExpression());
    }

    this->_parentModel->sendEntityToComponent(entity, this->getConnections()->getFrontConnection());
}