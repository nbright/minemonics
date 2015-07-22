//# corresponding header
#include <controller/universe/evolution/population/creature/Creature.hpp>

//# forward declarations
class SimulationManager;

//# system headers
//## controller headers
//## model headers
//## view headers
//# custom headers
//## base headers
#include <SimulationManager.hpp>

//## configuration headers
//## controller headers
#include <controller/universe/evolution/population/Population.hpp>
#include <controller/universe/evolution/population/creature/phenome/Phenome.hpp>

//## model headers
#include <model/universe/evolution/population/creature/CreatureModel.hpp>

//## view headers
//## utils headers

BoostLogger Creature::mBoostLogger; /*<! initialize the boost logger*/
Creature::_Init Creature::_initializer;
Creature::Creature(Population* const population, const Ogre::Vector3 position,
		const double branchiness) {
	// set up the creature model
	mCreatureModel = new CreatureModel();
	mCreatureModel->initialize(population->getPopulationModel(), position,
			branchiness);

	// set up the phenotype
	mPhenotype.initialize(this);

	mPhenotype.setPhenotypeModel(&mCreatureModel->getPhenotypeModel());

}

Creature::Creature(CreatureModel* const creatureModel) :
		mCreatureModel(creatureModel) {
	// set up the phenotype
	mPhenotype.initialize(this);

	mPhenotype.setPhenotypeModel(&mCreatureModel->getPhenotypeModel());
}

Creature::~Creature() {
	delete mCreatureModel;
//	mPhenotype
}

void Creature::performEmbryogenesis() {
	mPhenotype.performEmbryogenesis(mCreatureModel);
}

void Creature::reset(const Ogre::Vector3 position) {
	mCreatureModel->reset(position);
	mPhenotype.reset(position);
}

void Creature::reset() {
	reset(mCreatureModel->getInitialPosition());
}

void Creature::reposition(const Ogre::Vector3 position) {
	mCreatureModel->reposition(position);
	mPhenotype.reposition(position);
}

void Creature::update() {
	// Update the phenotype of the creature if it is in the world.
	if (mPhenotype.isInWorld()) {
		mPhenotype.update();
	}
}

void Creature::addToPhysicsWorld(){
	// develop creature if it is not developed yet.
	if (!isDeveloped()) {
		performEmbryogenesis();
	}

	// Add phenotype to world
	mPhenotype.addToPhysicsWorld();
}

void Creature::addToWorld() {
	// develop creature if it is not developed yet.
	if (!isDeveloped()) {
		performEmbryogenesis();
	}

	// Add phenotype to world
	mPhenotype.addToWorld();
}

void Creature::removeFromWorld() {
	// Remove phenotype from world
	mPhenotype.removeFromWorld();
}
