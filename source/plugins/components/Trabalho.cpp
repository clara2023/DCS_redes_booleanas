/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Trabalho.cpp
 * Author: rafael.luiz.cancian
 *
 * Created on 21 de Junho de 2018, 19:49
 */

#include "Trabalho.h"
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/Attribute.h"
#include "../../kernel/simulator/SimulationControlAndResponse.h"

#ifdef PLUGINCONNECT_DYNAMIC

extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &Trabalho::GetPluginInformation;
}
#endif

ModelDataDefinition* Trabalho::NewInstance(Model* model, std::string name) {
	return new Trabalho(model, name);
}

void Trabalho::setAllocation(Util::AllocationType allocation) {
	_allocation = allocation;
}

Util::AllocationType Trabalho::getAllocation() const {
	return _allocation;
}

Trabalho::Trabalho(Model* model, std::string name) : ModelComponent(model, Util::TypeOf<Trabalho>(), name) {
	SimulationControlGeneric<std::string>* propExpression = new SimulationControlGeneric<std::string>(
									std::bind(&Trabalho::trabalhoExpression, this), std::bind(&Trabalho::setTrabalhoExpression, this, std::placeholders::_1, Util::TimeUnit::unknown),
									Util::TypeOf<Trabalho>(), getName(), "TrabalhoExpression", "");
	SimulationControlGeneric<double>* propTrabalho = new SimulationControlGeneric<double>(
									std::bind(&Trabalho::delay, this), std::bind(&Trabalho::setTrabalho, this, std::placeholders::_1),
									Util::TypeOf<Trabalho>(), getName(), "Trabalho", "");
    SimulationControlGenericEnum<Util::TimeUnit, Util>* propUnitTime = new SimulationControlGenericEnum<Util::TimeUnit, Util>(
									std::bind(&Trabalho::delayTimeUnit, this),	std::bind(&Trabalho::setTrabalhoTimeUnit, this, std::placeholders::_1),
									Util::TypeOf<Trabalho>(), getName(), "TrabalhoTimeUnit", "");
    SimulationControlGenericEnum<Util::AllocationType, Util>* propAlloc = new SimulationControlGenericEnum<Util::AllocationType, Util>(
                                    std::bind(&Trabalho::getAllocation, this), std::bind(&Trabalho::setAllocation,  this, std::placeholders::_1),
                                    Util::TypeOf<Trabalho>(), getName(), "AllocationType", "");

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

void Trabalho::setTrabalho(double delay) {
	_delayExpression = std::to_string(delay);
}

double Trabalho::delay() const {
	return _parentModel->parseExpression(_delayExpression);
}

std::string Trabalho::show() {
	return ModelComponent::show() +
			",delayExpression=" + this->_delayExpression +
			",timeUnit=" + std::to_string(static_cast<int> (this->_delayTimeUnit));
}

//void Trabalho::setTrabalhoExpression(std::string _delayExpression) {
//	this->_delayExpression = _delayExpression;
//}

void Trabalho::setTrabalhoExpression(std::string _delayExpression, Util::TimeUnit _delayTimeUnit) {
	this->_delayExpression = _delayExpression;
	if (_delayTimeUnit != Util::TimeUnit::unknown) {
		this->_delayTimeUnit = _delayTimeUnit;
	}
}

std::string Trabalho::trabalhoExpression() const {
	return  _delayExpression;
}

void Trabalho::setTrabalhoTimeUnit(Util::TimeUnit _delayTimeUnit) {
	this->_delayTimeUnit = _delayTimeUnit;
}

Util::TimeUnit Trabalho::delayTimeUnit() const {
	return _delayTimeUnit;
}

void Trabalho::_onDispatchEvent(Entity* entity, unsigned int inputPortNumber) {
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

ModelComponent* Trabalho::LoadInstance(Model* model, PersistenceRecord *fields) {
	Trabalho* newComponent = new Trabalho(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

bool Trabalho::_loadInstance(PersistenceRecord *fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		_delayExpression = fields->loadField("delayExpression", DEFAULT.delayExpression);
		_delayTimeUnit = fields->loadField("delayExpressionTimeUnit", DEFAULT.delayTimeUnit);
		_allocation = static_cast<Util::AllocationType> (fields->loadField("allocation", static_cast<int> (DEFAULT.allocation)));
	}
	return res;
}

void Trabalho::_saveInstance(PersistenceRecord *fields, bool saveDefaultValues) {
	ModelComponent::_saveInstance(fields, saveDefaultValues);
	fields->saveField("delayExpression", this->_delayExpression, DEFAULT.delayExpression, saveDefaultValues);
	fields->saveField("delayExpressionTimeUnit", _delayTimeUnit, DEFAULT.delayTimeUnit, saveDefaultValues);
	fields->saveField("allocation", static_cast<int> (_allocation), static_cast<int> (DEFAULT.allocation), saveDefaultValues);
}

bool Trabalho::_check(std::string* errorMessage) {
	return _parentModel->checkExpression(_delayExpression, "Trabalho expression", errorMessage);
}

void Trabalho::_createInternalAndAttachedData() {
	if (_reportStatistics && _cstatWaitTime == nullptr) {
		_attachedAttributesInsert({"Entity.Total" + Util::StrAllocation(_allocation)+"Time"});
		_cstatWaitTime = new StatisticsCollector(_parentModel, getName() + "." + "TrabalhoTime", this);
		_internalDataInsert("TrabalhoTime", _cstatWaitTime);
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

PluginInformation* Trabalho::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Trabalho>(), &Trabalho::LoadInstance, &Trabalho::NewInstance);
	std::string text = "The Trabalho module delays an entity by a specified amount of time.";
	text += " When an entity arrives at a Trabalho module, the time delay expression is evaluated and the entity remains in the module for the resulting time.";
	text += " The time is then allocated to the entity’s value-added, non-value added, transfer, wait, or other time.";
	text += " Associated costs are calculated and allocated as well.";
	text += " TYPICAL USES: (1) Processing a check at a bank; (2) Performing a setup on a machine; (3) Transferring a document to another department";
	info->setDescriptionHelp(text);
	return info;
}
