#include "BooleanNetwork.h"
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/Attribute.h"
#include "../../kernel/simulator/SimulationControlAndResponse.h"

#ifdef PLUGINCONNECT_DYNAMIC

extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &BooleanNetwork::GetPluginInformation;
}
#endif

ModelDataDefinition* BooleanNetwork::NewInstance(Model* model, std::string name) {
	return new BooleanNetwork(model, name);
}

void BooleanNetwork::setAllocation(Util::AllocationType allocation) {
	_allocation = allocation;
}

Util::AllocationType BooleanNetwork::getAllocation() const {
	return _allocation;
}

BooleanNetwork::BooleanNetwork(Model* model, std::string name) : ModelComponent(model, Util::TypeOf<BooleanNetwork>(), name) {
	SimulationControlGeneric<std::string>* propExpression = new SimulationControlGeneric<std::string>(
									std::bind(&BooleanNetwork::booleanNetworkExpression, this), std::bind(&BooleanNetwork::setBooleanNetworkExpression, this, std::placeholders::_1, Util::TimeUnit::unknown),
									Util::TypeOf<BooleanNetwork>(), getName(), "BooleanNetworkExpression", "");
	SimulationControlGeneric<double>* propTrabalho = new SimulationControlGeneric<double>(
									std::bind(&BooleanNetwork::delay, this), std::bind(&BooleanNetwork::setBooleanNetwork, this, std::placeholders::_1),
									Util::TypeOf<BooleanNetwork>(), getName(), "BooleanNetwork", "");
    SimulationControlGenericEnum<Util::TimeUnit, Util>* propUnitTime = new SimulationControlGenericEnum<Util::TimeUnit, Util>(
									std::bind(&BooleanNetwork::delayTimeUnit, this),	std::bind(&BooleanNetwork::setBooleanNetworkTimeUnit, this, std::placeholders::_1),
									Util::TypeOf<BooleanNetwork>(), getName(), "BooleanNetworkTimeUnit", "");
    SimulationControlGenericEnum<Util::AllocationType, Util>* propAlloc = new SimulationControlGenericEnum<Util::AllocationType, Util>(
                                    std::bind(&BooleanNetwork::getAllocation, this), std::bind(&BooleanNetwork::setAllocation,  this, std::placeholders::_1),
                                    Util::TypeOf<BooleanNetwork>(), getName(), "AllocationType", "");

	_parentModel->getControls()->insert(propExpression);
	_parentModel->getControls()->insert(propTrabalho);
	_parentModel->getControls()->insert(propUnitTime);
    _parentModel->getControls()->insert(propAlloc);

	// setting properties
	_addProperty(propExpression);
	_addProperty(propTrabalho);
	_addProperty(propUnitTime);
    _addProperty(propAlloc);
}

void BooleanNetwork::setBooleanNetwork(double delay) {
	_delayExpression = std::to_string(delay);
}

double BooleanNetwork::delay() const {
	return _parentModel->parseExpression(_delayExpression);
}

std::string BooleanNetwork::show() {
	return ModelComponent::show() +
			",delayExpression=" + this->_delayExpression +
			",timeUnit=" + std::to_string(static_cast<int> (this->_delayTimeUnit));
}

//void Trabalho::setTrabalhoExpression(std::string _delayExpression) {
//	this->_delayExpression = _delayExpression;
//}

void BooleanNetwork::setBooleanNetworkExpression(std::string _delayExpression, Util::TimeUnit _delayTimeUnit) {
	this->_delayExpression = _delayExpression;
	if (_delayTimeUnit != Util::TimeUnit::unknown) {
		this->_delayTimeUnit = _delayTimeUnit;
	}
}

std::string BooleanNetwork::booleanNetworkExpression() const {
	return  _delayExpression;
}

void BooleanNetwork::setBooleanNetworkTimeUnit(Util::TimeUnit _delayTimeUnit) {
	this->_delayTimeUnit = _delayTimeUnit;
}

Util::TimeUnit BooleanNetwork::delayTimeUnit() const {
	return _delayTimeUnit;
}

void BooleanNetwork::_onDispatchEvent(Entity* entity, unsigned int inputPortNumber) {
	double waitTime = _parentModel->parseExpression(_delayExpression);
	Util::TimeUnit stu = _parentModel->getSimulation()->getReplicationBaseTimeUnit(); //getReplicationLengthTimeUnit();
	waitTime *= Util::TimeUnitConvert(_delayTimeUnit, stu);
	if (_reportStatistics) {
		std::string allocationCategory = Util::StrAllocation(_allocation);
		//try { //@TODO: What the hell????!!!
			_cstatWaitTime->getStatistics()->getCollector()->addValue(waitTime);
		//} catch (const std::exception& e) {
		//	traceError(e.what());
		//}
		if (entity->getEntityType()->isReportStatistics())
			entity->getEntityType()->addGetStatisticsCollector(entity->getEntityTypeName() + "." + allocationCategory+ "Time")->getStatistics()->getCollector()->addValue(waitTime);
		double totalWaitTime = entity->getAttributeValue("Entity.Total" + allocationCategory + "Time");
		std::string attribIndex="";
		entity->setAttributeValue("Entity.Total" + allocationCategory + "Time", totalWaitTime + waitTime, attribIndex, true);
	}
	double delayEndTime = _parentModel->getSimulation()->getSimulatedTime() + waitTime;
	Event* newEvent = new Event(delayEndTime, entity, this->getConnections()->getFrontConnection());
	_parentModel->getFutureEvents()->insert(newEvent);
	traceSimulation(this, "End of delay of "/*entity " + std::to_string(entity->entityNumber())*/ + entity->getName() + " scheduled to time " + std::to_string(delayEndTime) + Util::StrTimeUnitShort(stu) + " (wait time " + std::to_string(waitTime) + Util::StrTimeUnitShort(stu) + ") // " + _delayExpression+ " "+Util::StrTimeUnitShort(_delayTimeUnit));
}

ModelComponent* BooleanNetwork::LoadInstance(Model* model, PersistenceRecord *fields) {
	BooleanNetwork* newComponent = new BooleanNetwork(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

bool BooleanNetwork::_loadInstance(PersistenceRecord *fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		_delayExpression = fields->loadField("delayExpression", DEFAULT.delayExpression);
		_delayTimeUnit = fields->loadField("delayExpressionTimeUnit", DEFAULT.delayTimeUnit);
		_allocation = static_cast<Util::AllocationType> (fields->loadField("allocation", static_cast<int> (DEFAULT.allocation)));
	}
	return res;
}

void BooleanNetwork::_saveInstance(PersistenceRecord *fields, bool saveDefaultValues) {
	ModelComponent::_saveInstance(fields, saveDefaultValues);
	fields->saveField("delayExpression", this->_delayExpression, DEFAULT.delayExpression, saveDefaultValues);
	fields->saveField("delayExpressionTimeUnit", _delayTimeUnit, DEFAULT.delayTimeUnit, saveDefaultValues);
	fields->saveField("allocation", static_cast<int> (_allocation), static_cast<int> (DEFAULT.allocation), saveDefaultValues);
}

bool BooleanNetwork::_check(std::string* errorMessage) {
	return _parentModel->checkExpression(_delayExpression, "BooleanNetwork expression", errorMessage);
}

void BooleanNetwork::_createInternalAndAttachedData() {
	if (_reportStatistics && _cstatWaitTime == nullptr) {
		_attachedAttributesInsert({"Entity.Total" + Util::StrAllocation(_allocation)+"Time"});
		_cstatWaitTime = new StatisticsCollector(_parentModel, getName() + "." + "BooleanNetworkTime", this);
		_internalDataInsert("BooleanNetworkTime", _cstatWaitTime);
		// include StatisticsCollector needed in EntityType
		//ModelDataManager* elements = _parentModel->getDataManager();
		//std::list<ModelDataDefinition*>* enttypes = elements->getDataDefinitionList(Util::TypeOf<EntityType>())->list();
		//for (ModelDataDefinition* modeldatum : *enttypes) {
		//	EntityType* enttype = static_cast<EntityType*> (modeldatum);
		//	if (modeldatum->isReportStatistics())
		//		enttype->addGetStatisticsCollector(enttype->getName() + ".TrabalhoTime");
		//}
	} else {
		_internalDataClear();
		// @TODO remove StatisticsCollector needed in EntityType
	}
}

PluginInformation* BooleanNetwork::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<BooleanNetwork>(), &BooleanNetwork::LoadInstance, &BooleanNetwork::NewInstance);
	std::string text = "The BooleanNetwork module processes entities based on a boolean network structure.";
	text += " When an entity arrives at a BooleanNetwork module, the boolean network expression is evaluated and the entity is processed accordingly.";
	text += " The processing time is then allocated to the entity’s value-added, non-value added, transfer, wait, or other time.";
	text += " Associated costs are calculated and allocated as well.";
	text += " TYPICAL USES: (1) Processing a check at a bank; (2) Performing a setup on a machine; (3) Transferring a document to another department";
	info->setDescriptionHelp(text);
	return info;
}