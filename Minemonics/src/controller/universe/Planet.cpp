//# corresponding header
#include <controller/universe/Planet.hpp>

//# forward declarations
//# system headers
//## controller headers
//## model headers
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

//## view headers
//# custom headers
//## base headers
#include <SimulationManager.hpp>

//## configuration headers
//## controller headers
#include <controller/universe/Universe.hpp>
#include <controller/universe/evolution/Evolution.hpp>
#include <controller/universe/environments/Plane.hpp>

//## model headers
#include <model/universe/environments/EnvironmentModel.hpp>
#include <model/universe/environments/physics/PhysicsController.hpp>
#include <model/universe/PlanetModel.hpp>

//## view headers
//## utils headers

BoostLogger Planet::mBoostLogger; /*<! initialize the boost logger*/
Planet::_Init Planet::_initializer;
Planet::Planet() :
	mEnvironment(NULL), mPlanetModel(NULL) {
}
Planet::Planet(const PhysicsController::PhysicsModelType physicsModelType,
	const Environment::EnvironmentType type, const int evaluationTime,
	Ogre::Light* light) :
	mEnvironment(NULL) {
	mPlanetModel = new PlanetModel();

	//create earth evolution
	mEvolution.initialize(
		&SimulationManager::getSingleton()->getUniverse().getEvaluationController(),
		this, evaluationTime);

	// set up environment
	switch (type) {
	case Environment::HILLS: {
		//		mEnvironment = new Hills();
		//		((Hills*) mEnvironment)->initialize(physicsModelType, light);
		break;
	}
	case Environment::PLANE: {
		//create the terrain
		mEnvironment = new Plane();
		((Plane*) mEnvironment)->initialize(physicsModelType, light);
		break;
	}
	}
	mPlanetModel->initialize(mEvolution.getEvolutionModel(),
		mEnvironment->getEnvironmentModel());
}

Planet::Planet(PlanetModel* const planetModel) :
	mEnvironment(NULL), mPlanetModel(planetModel) {
}

Planet::~Planet() {
//	~mEvolution()
	delete mPlanetModel;
	mPlanetModel = NULL;

	delete mEnvironment;
	mEnvironment = NULL;
}

void Planet::addPopulation(Population* const population) {
	mEvolution.addPopulation(population);
}

void Planet::stepPhysics(const double timeSinceLastFrame) {
	mPlanetModel->getEnvironmentModel()->getPhysicsController()->stepBulletPhysics(
		timeSinceLastFrame);
}

void Planet::update(double timeSinceLastTick) {
	mEnvironment->update(timeSinceLastTick);
	mEvolution.update(timeSinceLastTick);
}

bool Planet::proceedEvaluation() {

	//if the evolution can not proceed, then remove the environment model from the world.
	bool canNotProceed = !mEvolution.proceedEvaluation();
	if (canNotProceed) {
		mEnvironment->removeFromWorld();
	}

	//TODO: The return value says if we are at the planet's end because it ran out of epochs.
	mPlanetModel->proceedEvaluation();
	return canNotProceed;
}

void Planet::drawDebugWorld() {
	//draws the debug world if it is enabled
	mPlanetModel->getEnvironmentModel()->getPhysicsController()->getDynamicsWorld()->debugDrawWorld();

}

void Planet::performEmbryogenesis() {
	mEvolution.performEmbryogenesis();
}

int Planet::getTotalCreatureQty() {
	return mEvolution.getTotalCreatureQty();
}

PhysicsController::PhysicsModelType Planet::getPhysicsModelType() {
	return mEnvironment->getEnvironmentModel()->getPhysicsController()->getPhysicsModelType();
}

void Planet::addEpoch(Epoch* epoch) {
	mPlanetModel->addEpoch(epoch);
}

void Planet::setSerializationPath(std::string serializationPath) {
	mPlanetModel->setSerializationPath(serializationPath);
}

std::string Planet::getSerializationPath() {
	return mPlanetModel->getSerializationPath();
}
