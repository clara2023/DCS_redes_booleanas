#include "BooleanNetwork.h"
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/Attribute.h"
#include "../../kernel/simulator/SimulationControlAndResponse.h"
#include "../../kernel/simulator/ModelComponent.h"
std::string BooleanNetwork::_userConfig = "";

#ifdef PLUGINCONNECT_DYNAMIC

extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &BooleanNetwork::GetPluginInformation;
}
#endif

void BooleanNetwork::configureFromArgs(int argc, char** argv) {
	for (int i = 0; i < argc; ++i) {
		std::string arg = argv[i];
		if (arg.find("--bn=") == 0) {
			// Remove the prefix "--bn="
			_userConfig = arg.substr(5);
		}
	}
	std::cout << "BooleanNetwork user config: " << _userConfig << std::endl;
}
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
	: ModelComponent(model, Util::TypeOf<BooleanNetwork>(), name) {
	// Estado inicial fixo para teste
	initializeNetwork("1101");
}

void BooleanNetwork::initializeNetwork(const std::string& initialState) {
	_state.clear();
	for (char c : initialState) {
		_state.push_back(c == '1');
	}
}

void BooleanNetwork::stepNetwork() {
	std::vector<bool> nextState = _state;
	for (size_t i = 0; i < _state.size(); ++i) {
		// Exemplo: NOT da célula atual
		nextState[i] = !_state[i];
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