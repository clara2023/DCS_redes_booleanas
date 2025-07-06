#ifndef BOOLEAN_NETWORK_H
#define BOOLEAN_NETWORK_H

#include <string>
#include "../../kernel/simulator/ModelComponent.h"
#include "../../kernel/simulator/Plugin.h"

/*!
Trabalho module
DESCRIPTION
This module is a complete copy of the Delay module, but with a different name and maybe I'll delete some of its functionality.
It is meant to be used as a starting point for a new module so that the other groups will have an easier time begining their own implementations.
TYPICAL USES
* Processing a check at a bank
* Performing a setup on a machine
* Transferring a document to another department
PROMPTS
Prompt Description
Name Unique module identifier displayed on the module shape.
Allocation Type of category to which the entity’s incurred delay time and
cost will be added.
Delay Time Determines the value of the delay for the entity.
Units Time units used for the delay time. 
 */
class BooleanNetwork : public ModelComponent {
public:
	BooleanNetwork(Model* model, std::string name = "");
	virtual ~BooleanNetwork() = default;
public:
	//void setDelayExpression(std::string _delayExpression);
	void setBooleanNetworkExpression(std::string _delayExpression, Util::TimeUnit _delayTimeUnit=Util::TimeUnit::unknown);
	void setBooleanNetwork(double delay);
    void setBooleanNetworkTimeUnit(Util::TimeUnit _delayTimeUnit);
    Util::TimeUnit booleanNetworkTimeUnit() const;
    std::string booleanNetworkExpression() const;
	void setDelay(double delay);
	double delay() const;
	void setDelayTimeUnit(Util::TimeUnit _delayTimeUnit);
	Util::TimeUnit delayTimeUnit() const;
    void setAllocation(Util::AllocationType allocation);
    Util::AllocationType getAllocation() const;
public:
	virtual std::string show();
public:
	static PluginInformation* GetPluginInformation();
	static ModelComponent* LoadInstance(Model* model, PersistenceRecord *fields);
	static ModelDataDefinition* NewInstance(Model* model, std::string name = "");
//public: // properties
	//using property_owner_t = Delay;
	//decl_property(DelayExpression, decl_get(std::string){return delayExpression();} void decl_set(std::string val){setDelayExpression(val);});

protected:
	virtual void _onDispatchEvent(Entity* entity, unsigned int inputPortNumber);
	virtual bool _loadInstance(PersistenceRecord *fields);
	virtual void _saveInstance(PersistenceRecord *fields, bool saveDefaultValues);
protected:
	//virtual void _initBetweenReplications();
	virtual bool _check(std::string* errorMessage);
	virtual void _createInternalAndAttachedData();
public:
	const struct DEFAULT_VALUES {
		const std::string delayExpression = "1.0";
		const Util::TimeUnit delayTimeUnit = Util::TimeUnit::second;
		const Util::AllocationType allocation = Util::AllocationType::Wait; 
		
	} DEFAULT;
private:
	std::string _delayExpression = DEFAULT.delayExpression;
	Util::TimeUnit _delayTimeUnit = DEFAULT.delayTimeUnit;
	Util::AllocationType _allocation = DEFAULT.allocation;
private: // inner internal elements
	StatisticsCollector* _cstatWaitTime = nullptr;
};
//enable_this_owner(Delay, DelayExpression);

#endif /* BOOLEAN_NETWORK_H */
