#pragma once

#include "../../kernel/simulator/ModelComponent.h"
#include "BooleanNetworkNode.h"

class BooleanNetwork : public ModelComponent {
public:
    BooleanNetwork(Model* model, std::string name = "");
    virtual ~BooleanNetwork() = default;

    void addNode(BooleanNetworkNode* node);
    std::vector<BooleanNetworkNode*> getNodes() const;

    static PluginInformation* GetPluginInformation();
    static ModelComponent* LoadInstance(Model* model, PersistenceRecord *fields);
    static ModelDataDefinition* NewInstance(Model* model, std::string name = "");

    virtual std::string show();

protected:
    virtual bool _loadInstance(PersistenceRecord *fields) override;
    virtual void _saveInstance(PersistenceRecord *fields, bool saveDefaultValues) override;
    virtual void _onDispatchEvent(Entity* entity, unsigned int inputPortNumber) override;

private:
    std::vector<BooleanNetworkNode*> _nodes;
};