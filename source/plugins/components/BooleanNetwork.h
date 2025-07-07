#ifndef BOOLEANNETWORK_H
#define BOOLEANNETWORK_H

#include "../../kernel/simulator/ModelComponent.h"

class BooleanNetwork : public ModelComponent {
public:
	static void configureFromArgs(int argc, char** argv);
	static std::string getUserConfig();
	BooleanNetwork(Model* model, std::string name = "");
	virtual ~BooleanNetwork() = default;

public:
	static ModelComponent* LoadInstance(Model* model, PersistenceRecord *fields);
	static ModelDataDefinition* NewInstance(Model* model, std::string name);
	static PluginInformation* GetPluginInformation();

public:
	void initializeNetwork(const std::string& initialState);
	void stepNetwork();

protected:
	virtual void _onDispatchEvent(Entity* entity, unsigned int inputPortNumber) override;
	virtual bool _loadInstance(PersistenceRecord *fields) override;
	virtual void _saveInstance(PersistenceRecord *fields, bool saveDefaultValues) override;

private:
	std::vector<bool> _state;
	static std::string _userConfig;
};

#endif /* BOOLEANNETWORK_H */
