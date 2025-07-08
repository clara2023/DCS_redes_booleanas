#pragma once

#include "../../kernel/simulator/PluginInformation.h"
#include "../../kernel/simulator/ModelDataDefinition.h"
#include <string>
#include <vector>


class BooleanNetworkNode : public ModelDataDefinition {
public:
	BooleanNetworkNode(Model* model, std::string name = "");
	virtual ~BooleanNetworkNode() = default;
	static PluginInformation* GetPluginInformation();
	static ModelDataDefinition* LoadInstance(Model* model, PersistenceRecord *fields);
	static ModelDataDefinition* NewInstance(Model* model, std::string name);

	void setExpression(const std::string& expr);
	std::string getExpression() const;

	void setInputs(const std::vector<std::string>& inputs);
	std::vector<std::string> getInputs() const;

	void setValue(bool value);
	bool getValue() const;


	virtual std::string show();

protected:
	virtual void _onDispatchEvent(Entity* entity, unsigned int inputPortNumber);
	virtual bool _loadInstance(PersistenceRecord *fields);
	virtual void _saveInstance(PersistenceRecord *fields, bool saveDefaultValues);

private:
	std::string _expression;
	std::vector<std::string> _inputs;
	bool _value = false;
};