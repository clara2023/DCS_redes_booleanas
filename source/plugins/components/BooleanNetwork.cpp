#include "BooleanNetwork.h"
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/Parser_if.h"
#include "../../kernel/simulator/PluginManager.h"
#include "../../kernel/simulator/Attribute.h"
#include "../../kernel/simulator/SimulationControlAndResponse.h"
#include "../../kernel/simulator/ModelComponent.h"

#ifdef PLUGINCONNECT_DYNAMIC

extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &BooleanNetwork::GetPluginInformation;
}
#endif

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
	// attributes each node with its current state
	for (size_t i = 0; i < _state.size(); ++i) {
		std::string nodeName = "N" + std::to_string(i);
		entity->setAttributeValue(nodeName, _state[i] ? 1.0 : 0.0, "0", true); // Store as 1.0 or 0.0
	}
	// Passa a entidade adiante sem atraso
	_parentModel->sendEntityToComponent(entity, this->getConnections()->getFrontConnection());
}

void BooleanNetwork::initializeNetwork(const std::string& initialState) {
	_state.clear();
	for (char c : initialState) {
		_state.push_back(c == '1');
	}
}

bool BooleanNetwork::_loadInstance(PersistenceRecord *fields) {
	ModelComponent::_loadInstance(fields);
	return true;
}

void BooleanNetwork::_saveInstance(PersistenceRecord *fields, bool saveDefaultValues) {
	ModelComponent::_saveInstance(fields, saveDefaultValues);
}

PluginInformation* BooleanNetwork::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<BooleanNetwork>(), &BooleanNetwork::LoadInstance, &BooleanNetwork::NewInstance);
	std::string text = "BooleanNetwork é um componente que emula um passo de rede booleana.";
	text += " O resultado, ou seja, o estado da rede, é uma string atualizada com base nas expressões booleanas fornecidas.";
	text += " As expressões devem ser fornecidas na forma de strings, onde cada nodo é representada por 'N{i}',";
	text += " onde 'i' é o índice do nodo na rede. O estado inicial da rede é definido por uma string de 0s e 1s,";
	text += " onde '1' representa um nodo ativo e '0' um nodo inativo. O componente processa as expressões booleanas";
	text += " e atualiza o estado da rede a cada passo. As expressões podem incluir operadores lógicos como AND, OR, NOT,";
	text += " e podem ser combinadas para criar redes booleanas complexas.";

	info->setDescriptionHelp(text);
	return info;
}