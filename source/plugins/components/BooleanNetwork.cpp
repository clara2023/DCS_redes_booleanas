#include "BooleanNetwork.h"
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/Parser_if.h"
#include "../../kernel/simulator/PluginManager.h"
#include "../../kernel/simulator/Attribute.h"
#include "../../kernel/simulator/SimulationControlAndResponse.h"
#include "../../kernel/simulator/ModelComponent.h"
std::string BooleanNetwork::_userConfig = "";

#ifdef PLUGINCONNECT_DYNAMIC

extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &BooleanNetwork::GetPluginInformation;
}
#endif

std::string BooleanNetwork::getUserConfig() {
	return _userConfig;
}

ModelDataDefinition* BooleanNetwork::NewInstance(Model* model, std::string name) {
	return new BooleanNetwork(model, name);
}

ModelComponent* BooleanNetwork::LoadInstance(Model* model, PersistenceRecord *fields) {
	BooleanNetwork* newComponent = new BooleanNetwork(model);
	newComponent->_loadInstance(fields); // opcional
	return newComponent;
}

BooleanNetwork::BooleanNetwork(Model* model, std::string name)
: ModelComponent(model, Util::TypeOf<BooleanNetwork>(), name) {}

void BooleanNetwork::initializeNetwork(const std::string& initialState) {
	_state.clear();
	for (char c : initialState) {
		_state.push_back(c == '1');
	}
}

void BooleanNetwork::setExpression(std::vector<std::string> expr) {
	_expression = expr;
}

std::vector<std::string> BooleanNetwork::_getExprs() const {
	// replaces each "N{i}" with the value of _state[i]
	std::vector<std::string> modifiedExpressions = _expression;
	for (size_t i = 0; i < _state.size(); ++i) {
		// Finds each "N{i}" with the value of _state[i]
		// and replaces it with "1" or "0" in modifiedExpressions
		std::string placeholder = "N" + std::to_string(i);
		for (std::string& expr : modifiedExpressions) {
			size_t pos = 0;
			while ((pos = expr.find(placeholder, pos)) != std::string::npos) {
				expr.replace(pos, placeholder.length(), _state[i] ? "1" : "0");
				pos += 1; // Move past the replaced character
			}
		}
	}
	return modifiedExpressions;
}

void BooleanNetwork::stepNetwork() {
	std::vector<bool> nextState = _state;
	std::vector<std::string> expressions = _getExprs();
	for (size_t i = 0; i < expressions.size(); ++i) {
		bool result = _parentModel->parseExpression(expressions[i]);
		nextState[i] = (result != 0.0); // Assuming the expression evaluates to a boolean value
	}
	_state = nextState;
}

void BooleanNetwork::_onDispatchEvent(Entity* entity, unsigned int inputPortNumber) {
	stepNetwork();
	// TODO: remove this debug output maybe
	std::cout << "Entity " << entity->getName() << " passou pelo BooleanNetwork. Estado: ";
	for (bool b : _state) std::cout << (b ? '1' : '0');
	std::cout << std::endl;

	// Passa a entidade adiante sem atraso
	_parentModel->sendEntityToComponent(entity, this->getConnections()->getFrontConnection());
}

bool BooleanNetwork::_loadInstance(PersistenceRecord *fields) {
	ModelComponent::_loadInstance(fields);
	return true;
}

void BooleanNetwork::_saveInstance(PersistenceRecord *fields, bool saveDefaultValues) {
	ModelComponent::_saveInstance(fields, saveDefaultValues);
}

PluginInformation* BooleanNetwork::GetPluginInformation() {
	//TODO: change description
	PluginInformation* info = new PluginInformation(Util::TypeOf<BooleanNetwork>(), &BooleanNetwork::LoadInstance, &BooleanNetwork::NewInstance);
	std::string text = "The BooleanNetwork module processes entities based on a boolean network structure.";
	text += " When an entity arrives at a BooleanNetwork module, the boolean network expression is evaluated and the entity is processed accordingly.";
	text += " The processing time is then allocated to the entity’s value-added, non-value added, transfer, wait, or other time.";
	text += " Associated costs are calculated and allocated as well.";
	text += " TYPICAL USES: (1) Processing a check at a bank; (2) Performing a setup on a machine; (3) Transferring a document to another department";
	info->setDescriptionHelp(text);
	return info;
}