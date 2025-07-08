#include "BooleanNetworkNode.h"
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/PluginManager.h"

BooleanNetworkNode::BooleanNetworkNode(Model* model, std::string name)
    : ModelDataDefinition(model, Util::TypeOf<BooleanNetworkNode>(), name) {}

void BooleanNetworkNode::setExpression(const std::string& expr) {
    _expression = expr;
}

std::string BooleanNetworkNode::getExpression() const {
    return _expression;
}

void BooleanNetworkNode::setInputs(const std::vector<std::string>& inputs) {
    _inputs = inputs;
}

std::vector<std::string> BooleanNetworkNode::getInputs() const {
    return _inputs;
}

void BooleanNetworkNode::setValue(bool value) {
    _value = value;
}

bool BooleanNetworkNode::getValue() const {
    return _value;
}

std::string BooleanNetworkNode::show() {
    std::string txt = ModelDataDefinition::show();
    txt += ", expression=\"" + _expression + "\", inputs=[";

    for (const std::string& input : _inputs) {
        txt += input + ",";
    }

    if (!_inputs.empty())
        txt = txt.substr(0, txt.length() - 1);

    txt += "], value=" + std::string(_value ? "true" : "false");
    return txt;
}

ModelDataDefinition* BooleanNetworkNode::NewInstance(Model* model, std::string name) {
    return new BooleanNetworkNode(model, name);
}

PluginInformation* BooleanNetworkNode::GetPluginInformation() {
    PluginInformation* info = new PluginInformation(Util::TypeOf<BooleanNetworkNode>(), &BooleanNetworkNode::LoadInstance, &BooleanNetworkNode::NewInstance);
    return info;
}

ModelDataDefinition* BooleanNetworkNode::LoadInstance(Model* model, PersistenceRecord *fields) {
	BooleanNetworkNode* newElement = new BooleanNetworkNode(model);
	try {
		newElement->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newElement;
}

void BooleanNetworkNode::_onDispatchEvent(Entity* entity, unsigned int inputPortNumber) {
    return;
}

bool BooleanNetworkNode::_loadInstance(PersistenceRecord* fields) {
    bool res = ModelDataDefinition::_loadInstance(fields);
    if (res) {
        _expression = fields->loadField("expression", "");

        unsigned int inputCount = fields->loadField("inputsCount", 0u);
        _inputs.clear();
        for (unsigned int i = 0; i < inputCount; ++i) {
            std::string inputName = fields->loadField("input_" + std::to_string(i), "");
            _inputs.push_back(inputName);
        }

        unsigned int valueAsInt = fields->loadField("value", 0u);
        _value = valueAsInt != 0;
    }
    return res;
}

void BooleanNetworkNode::_saveInstance(PersistenceRecord* fields, bool saveDefaultValues) {
    ModelDataDefinition::_saveInstance(fields, saveDefaultValues);

    fields->saveField("expression", _expression, "", saveDefaultValues);
    fields->saveField("inputsCount", _inputs.size(), 0u, saveDefaultValues);

    for (unsigned int i = 0; i < _inputs.size(); ++i) {
        fields->saveField("input_" + std::to_string(i), _inputs[i], "", saveDefaultValues);
    }

    fields->saveField("value", _value ? 1u : 0u, 0u, saveDefaultValues);
}
