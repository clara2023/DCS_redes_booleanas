#include "Smart_BooleanNetwork.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../../../kernel/simulator/Simulator.h"

// Model Components
#include "../../../../plugins/components/Create.h"
#include "../../../../plugins/components/Seize.h"
#include "../../../../plugins/components/BooleanNetwork.h"
#include "../../../../plugins/components/Release.h"
#include "../../../../plugins/components/Dispose.h"
#include "../../../TraitsApp.h"

Smart_BooleanNetwork::Smart_BooleanNetwork() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Smart_BooleanNetwork::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	genesys->getTracer()->setTraceLevel(TraitsApp<GenesysApplication_if>::traceLevel);
	setDefaultTraceHandlers(genesys->getTracer());
	PluginManager* plugins = genesys->getPlugins();
	plugins->autoInsertPlugins("autoloadplugins.txt");
	Model* model = genesys->getModels()->newModel();
	// create model
	Create* create1 = plugins->newInstance<Create>(model);
	create1->setEntityTypeName("Customer");
	create1->setTimeBetweenCreationsExpression("expo(20)");
	create1->setTimeUnit(Util::TimeUnit::second);
	create1->setEntitiesPerCreation(1);
	Resource* machine1 = plugins->newInstance<Resource>(model, "Machine_1");
	machine1->setCapacity(1);
	Queue* queueSeize1 = plugins->newInstance<Queue>(model, "Seize_1.Queue");
	queueSeize1->setOrderRule(Queue::OrderRule::FIFO);
	Seize* seize1 = plugins->newInstance<Seize>(model);
	seize1->getSeizeRequests()->insert(new SeizableItem(machine1, "1"));
	seize1->setQueue(queueSeize1);
	seize1->setAllocationType(Util::AllocationType::Transfer);

	// BooleanNetwork element created
	BooleanNetwork* booleanNet = plugins->newInstance<BooleanNetwork>(model);
	booleanNet->initializeNetwork("1010");
	std::cout << "Initial state: ";
	booleanNet->setExpression({"N1==0", "N1*N2", "N2+N3", "N2+N3 == 1"}); // Example expression

	Release* release1 = plugins->newInstance<Release>(model);
	release1->getReleaseRequests()->insert(new SeizableItem(machine1, "1"));
	Dispose* dispose1 = plugins->newInstance<Dispose>(model);
	// connect model components to create a "workflow"
	create1->getConnections()->insert(seize1);
	seize1->getConnections()->insert(booleanNet);
	booleanNet->getConnections()->insert(release1);
	release1->getConnections()->insert(dispose1);
	// set options, save and simulate
	ModelSimulation* sim = model->getSimulation();
	sim->setReplicationLength(1000, Util::TimeUnit::second);
	sim->setNumberOfReplications(1); // TODO: check replication number
	model->getTracer()->setTraceLevel(TraceManager::Level::L2_results);
	model->save("./models/Smart_BooleanNetwork.gen");
	// execute the simulation
	sim->start();
	delete genesys;
	return 0;
};
