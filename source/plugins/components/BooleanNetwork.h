#pragma once

#include "../../kernel/simulator/ModelComponent.h"

class BooleanNetwork : public ModelComponent {
public:
BooleanNetwork(Model* model, std::string name = "");
virtual ~BooleanNetwork() = default;
static std::string getUserConfig();

public:
	static PluginInformation* GetPluginInformation();
	static ModelComponent* LoadInstance(Model* model, PersistenceRecord *fields);
	static ModelDataDefinition* NewInstance(Model* model, std::string name);

public:
	void setExpression(std::vector<std::string> expr);
	void initializeNetwork(const std::string& initialState);
	void stepNetwork();

protected:
	virtual void _onDispatchEvent(Entity* entity, unsigned int inputPortNumber) override;
	virtual bool _loadInstance(PersistenceRecord *fields) override;
	virtual void _saveInstance(PersistenceRecord *fields, bool saveDefaultValues) override;

private:
	std::vector<bool> _state;
	static std::string _userConfig;
	std::vector<std::string>  _expression; // This could be used to store a boolean expression for the network, if needed
	std::vector<std::string> _getExprs() const;
};
